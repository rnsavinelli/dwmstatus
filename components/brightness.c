/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>

#include "../util.h"

#if defined(__linux__)
    #include <limits.h>

    const char *
    brightness(const char *interface)
    {
        char path[PATH_MAX];
        char *dataread;
        int actual_brightness = 0, max_brightness = 1;

    	if (esnprintf(path, sizeof(path), "/sys/class/backlight/%s/",
    		          interface) < 0) {
	    	return 0;
	    }

        dataread = readfile(path, "actual_brightness");
        if (dataread != NULL) {
            actual_brightness = atoi(dataread);
        }
        free(dataread);

        dataread = readfile(path, "max_brightness");
        if (dataread != NULL) {
            max_brightness = atoi(dataread);
        }
        free(dataread);

        return bprintf("%d", (actual_brightness * 100) / max_brightness);
    }
#endif
