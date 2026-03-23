#include <stddef.h>
#include <stdint.h>

enum {
    VGA_WIDTH = 80,
    VGA_HEIGHT = 25,
    VGA_COLOR = 0x0F,
};

static volatile uint16_t* const vga_buffer = (uint16_t*)0xB8000;
static size_t cursor_row = 0;
static size_t cursor_column = 0;

static uint16_t vga_entry(char character, uint8_t color)
{
    return (uint16_t)character | ((uint16_t)color << 8);
}

static void clear_screen(void)
{
    size_t index;

    for (index = 0; index < VGA_WIDTH * VGA_HEIGHT; ++index) {
        vga_buffer[index] = vga_entry(' ', VGA_COLOR);
    }

    cursor_row = 0;
    cursor_column = 0;
}

static void put_char(char character)
{
    if (character == '\n') {
        cursor_column = 0;
        if (cursor_row + 1 < VGA_HEIGHT) {
            ++cursor_row;
        }
        return;
    }

    vga_buffer[cursor_row * VGA_WIDTH + cursor_column] = vga_entry(character, VGA_COLOR);
    ++cursor_column;

    if (cursor_column >= VGA_WIDTH) {
        cursor_column = 0;
        if (cursor_row + 1 < VGA_HEIGHT) {
            ++cursor_row;
        }
    }
}

static void write_line(const char* text)
{
    while (*text != '\0') {
        put_char(*text);
        ++text;
    }
    put_char('\n');
}

void kernel_main(uint32_t magic, uint32_t multiboot_info)
{
    (void)multiboot_info;

    clear_screen();

    write_line("VelocityOS");
    write_line("microkernel roadmap scaffold");
    write_line("");

    if (magic == 0x2BADB002) {
        write_line("[ OK ] multiboot handshake");
    } else {
        write_line("[ERR ] invalid bootloader state");
    }

    write_line("[ OK ] VGA text console");
    write_line("[TODO] GDT + IDT");
    write_line("[TODO] physical memory manager");
    write_line("[TODO] scheduler + IPC");
    write_line("");
    write_line("velocity@kernel:~$");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
