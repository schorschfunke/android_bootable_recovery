#include <stdio.h>

#include <cutils/properties.h>
#include <sys/system_properties.h>
#include "../../system/core/toolbox/dynarray.h"

int setprop_main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(stderr,"usage: setprop <key> <value>\n");
        return 1;
    }

    if(property_set(argv[1], argv[2])){
        fprintf(stderr,"could not set property\n");
        return 1;
    }

    return 0;
}

static void record_prop(const char* key, const char* name, void* opaque)
{
    strlist_t* list = opaque;
    char temp[PROP_VALUE_MAX + PROP_NAME_MAX + 16];
    snprintf(temp, sizeof temp, "[%s]: [%s]", key, name);
    strlist_append_dup(list, temp);
}

static void list_properties(void)
{
    strlist_t list[1] = { STRLIST_INITIALIZER };

    /* Record properties in the string list */
    (void)property_list(record_prop, list);

    /* Sort everything */
    strlist_sort(list);

    /* print everything */
    STRLIST_FOREACH(list, str, printf("%s\n", str));

    /* voila */
    strlist_done(list);
}

int __system_property_wait(prop_info *pi);

int getprop_main(int argc, char *argv[])
{
    int n = 0;

    if (argc == 1) {
        list_properties();
    } else {
        char value[PROPERTY_VALUE_MAX];
        char *default_value;
        if(argc > 2) {
            default_value = argv[2];
        } else {
            default_value = "";
        }

        property_get(argv[1], value, default_value);
        printf("%s\n", value);
    }
    return 0;
}
