const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{
        .preferred_optimize_mode = .ReleaseSmall,
    });

    const module = b.createModule(.{
        .target = b.resolveTargetQuery(.{
            .cpu_arch = .x86,
            .os_tag = .freestanding,
        }),
        .optimize = optimize,
    });

    const kernel = b.addExecutable(.{
        .name = "kernel.bin",
        .root_module = module,
    });
    kernel.addAssemblyFile(b.path("boot/multiboot.s"));
    kernel.addCSourceFiles(.{
        .files = &.{
            "src/kernel/main.c",
            "src/kernel/tty.c",
            "src/kernel/serial.c",
            "src/kernel/console.c",
            "src/kernel/keyboard.c",
            "src/kernel/ps2.c",
            "src/arch/gdt.c",
            "src/arch/interrupts.c",
            "src/arch/irq.c",
            "src/lib/memory.c",
        },
    });
    kernel.addIncludePath(b.path("src/"));
    kernel.setLinkerScript(b.path("link.ld"));

    const copy = b.addInstallArtifact(kernel, .{
        .dest_dir = .{ .override = .{ .custom = "grub/boot/" } },
    });

    const iso = b.addSystemCommand(&.{
        "grub-mkrescue",
        "-o",
        "out/os.iso",
        "zig-out/grub/",
    });

    iso.step.dependOn(&copy.step);

    b.getInstallStep().dependOn(&iso.step);
}
