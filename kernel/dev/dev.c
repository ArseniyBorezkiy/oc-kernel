#include <dev/dev.h>
#include <lib/assert.h>
#include <lib/data/clist.h>
#include <lib/stdlib.h>
#include <lib/string.h>

static bool dev_by_name_detector(struct clist_head_t* current, va_list args);

struct clist_definition_t dev_list = {
    .head = null,
    .slot_size = sizeof(struct dev_t)
};

/*
 * Api - init module
 */
extern void dev_init()
{
    /* init data structures */
    dev_list.head = null;
}

/*
 * Api - Device list
 */
extern struct clist_definition_t *dev_get_dev_list() {
    return &dev_list;
}

/*
 * Api - register device
 */
extern void dev_register(struct dev_t* dev)
{
    struct clist_head_t* entry;
    struct dev_t* device;

    /* create list entry */
    entry = clist_insert_entry_after(&dev_list, dev_list.head);
    device = (struct dev_t*)entry->data;

    /* fill data */
    strncpy(device->name, dev->name, sizeof(dev->name));
    device->base_r = dev->base_r;
    device->base_w = dev->base_w;
    device->read_cb = dev->read_cb;
    device->write_cb = dev->write_cb;
    device->ioctl_cb = dev->ioctl_cb;
    device->ih_list.head = dev->ih_list.head;
    device->ih_list.slot_size = dev->ih_list.slot_size;
}

/*
 * Api - find device by name
 */
extern struct dev_t* dev_find_by_name(char* name)
{
    struct clist_head_t* entry;
    entry = clist_find(&dev_list, dev_by_name_detector, name);
    return (struct dev_t*)entry->data;
}

/*
 * Api - device for each
 */
extern void dev_for_each(dev_each_callback_t callback, void* data)
{
    struct clist_head_t* current;
    struct dev_t* dev;

    for (current = dev_list.head; current != null; current = current->next) {
        dev = (struct dev_t*)current->data;
        callback(dev, data);

        if (current->next == dev_list.head) {
            break;
        }
    }
}

/*
 * Device by name detector
 */
static bool dev_by_name_detector(struct clist_head_t* current, va_list args)
{
    char* name = va_arg(args, char*);
    struct dev_t* dev = (struct dev_t*)current->data;
    return !strcmp(name, dev->name);
}
