#include <dev/dev.h>
#include <lib/string.h>
#include <lib/assert.h>
#include <lib/stdlib.h>
#include <lib/data/clist.h>

static bool dev_by_name_detector(struct clist_head_t *current, va_list args);

struct clist_definition_t dev_list = {
    .head = null,
    .slot_size = sizeof(struct dev_t)};

/*
 * Api - init module
 */
extern void dev_init()
{
  /* init data structures */
  dev_list.head = null;
}

/*
 * Api - register device
 */
extern void dev_register(struct dev_t *dev)
{
  struct dev_t *entry;

  /* create list entry */
  entry = clist_insert_entry_after(&dev_list, dev_list.head);

  /* fill data */
  strncpy(entry->name, dev->name, sizeof(dev->name));
  entry->base_r = dev->base_r;
  entry->base_w = dev->base_w;
  entry->read_cb = dev->read_cb;
  entry->write_cb = dev->write_cb;
  entry->ioctl_cb = dev->ioctl_cb;
  entry->ih_list.head = dev->ih_list.head;
  entry->ih_list.slot_size = dev->ih_list.slot_size;
}

/*
 * Api - find device by name
 */
extern struct dev_t *dev_find_by_name(char *name)
{
  return clist_find(&dev_list, dev_by_name_detector, name);
}

/*
 * Api - device for each
 */
extern void dev_for_each(dev_each_callback_t callback, void *data)
{
  struct clist_head_t *current;
  struct dev_t *dev;

  for (current = dev_list.head; current != null; current = current->next)
  {
    dev = (struct dev_t *)current->data;
    callback(dev, data);

    if (current->next == dev_list.head)
    {
      break;
    }
  }
}

/*
 * Device by name detector
 */
static bool dev_by_name_detector(struct clist_head_t *current, va_list args)
{
  char *name = va_arg(args, char *);
  struct dev_t *dev = (struct dev_t *)current->data;
  return !strcmp(name, dev->name);
}
