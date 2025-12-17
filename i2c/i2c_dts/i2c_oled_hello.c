// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32

// Simple 5x7 font for ASCII 32-127
static const u8 font5x7[][5] = {
    // Only defining H, E, L, O for simplicity
    ['H'-32] = {0x7F,0x08,0x08,0x08,0x7F},
    ['E'-32] = {0x7F,0x49,0x49,0x49,0x41},
    ['L'-32] = {0x7F,0x40,0x40,0x40,0x40},
    ['O'-32] = {0x3E,0x41,0x41,0x41,0x3E},
};

// Initialization commands for 128x32
static const u8 ssd1306_init_cmds[] = {
    0xAE,       // Display OFF
    0xD5, 0x80, // Clock divide
    0xA8, 0x1F, // Multiplex 32
    0xD3, 0x00, // Display offset
    0x40,       // Start line
    0x8D, 0x14, // Charge pump ON
    0x20, 0x00, // Horizontal addressing
    0xA1,       // Segment remap
    0xC8,       // COM scan direction
    0xDA, 0x02, // COM pins 128x32
    0x81, 0x7F, // Contrast
    0xAF        // Display ON
};

// Send a single command
static int ssd1306_send_cmd(struct i2c_client *client, u8 cmd)
{
    u8 buf[2] = {0x00, cmd};
    return i2c_master_send(client, buf, 2);
}

// Send an array of commands
static int ssd1306_send_cmds(struct i2c_client *client,
                             const u8 *cmds, int size)
{
    int i, ret;
    for (i = 0; i < size; i++) {
        ret = ssd1306_send_cmd(client, cmds[i]);
        if (ret < 0)
            return ret;
        udelay(10);
    }
    return 0;
}

// Send a data byte
static int ssd1306_send_data(struct i2c_client *client, u8 data)
{
    u8 buf[2] = {0x40, data}; // 0x40 = data control byte
    return i2c_master_send(client, buf, 2);
}

// Write "HELLO" using font5x7
static int ssd1306_write_hello(struct i2c_client *client)
{
    const char *str = "HELLO";
    int i, j;
    int ret;

    for (i = 0; str[i]; i++) {
        char c = str[i];
        for (j = 0; j < 5; j++) { // 5 bytes per char
            ret = ssd1306_send_data(client, font5x7[c-32][j]);
            if (ret < 0) return ret;
        }
        ret = ssd1306_send_data(client, 0x00); // 1 column space
        if (ret < 0) return ret;
    }
    return 0;
}

static int ssd1306_probe(struct i2c_client *client)
{
    int ret;

    dev_info(&client->dev, "SSD1306 128x32 OLED detected at 0x%02x\n",
             client->addr);

    ret = ssd1306_send_cmds(client, ssd1306_init_cmds,
                            sizeof(ssd1306_init_cmds));
    if (ret < 0) return ret;

    ret = ssd1306_write_hello(client);
    if (ret < 0) return ret;

    dev_info(&client->dev, "HELLO written to OLED\n");
    return 0;
}

static void ssd1306_remove(struct i2c_client *client)
{
    dev_info(&client->dev, "SSD1306 OLED removed\n");
}

static const struct of_device_id ssd1306_dt_ids[] = {
    { .compatible = "mycompany,my-i2c-oled" },
    { }
};
MODULE_DEVICE_TABLE(of, ssd1306_dt_ids);

static struct i2c_driver ssd1306_driver = {
    .driver = {
        .name = "oled",
        .of_match_table = ssd1306_dt_ids,
    },
    .probe = ssd1306_probe,
    .remove = ssd1306_remove,
};

module_i2c_driver(ssd1306_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SSD1306 128x32 OLED HELLO Driver");
