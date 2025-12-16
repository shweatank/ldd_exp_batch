// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>

static const u8 ssd1306_init_cmds[] = {
    0x00,           // Command stream
    0xAE,           // Display OFF
    0x20, 0x00,     // Memory Mode = Horizontal
    0xA1,           // Segment Remap
    0xC8,           // COM Scan Dec
    0xA8, 0x3F,     // Multiplex ratio 63
    0xD3, 0x00,     // Display offset
    0x40,           // Start line = 0
    0x8D, 0x14,     // Charge pump ON
    0xAF            // Display ON
};

static int ssd1306_send_cmds(struct i2c_client *client,
                             const u8 *cmds, int size)
{
    int ret = i2c_master_send(client, cmds, size);
    if (ret < 0)
        dev_err(&client->dev, "cmd send failed: %d\n", ret);
    return ret;
}

static int ssd1306_probe(struct i2c_client *client)
{
    int ret;

    dev_info(&client->dev, "SSD1306 OLED detected at 0x%02x\n",
             client->addr);

    ret = ssd1306_send_cmds(client, ssd1306_init_cmds,
                            sizeof(ssd1306_init_cmds));
    if (ret < 0)
        return ret;

    dev_info(&client->dev, "SSD1306 OLED initialized\n");
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
MODULE_DESCRIPTION("Minimal SSD1306 OLED Driver");

