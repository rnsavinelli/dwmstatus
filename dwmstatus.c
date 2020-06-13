#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <X11/Xlib.h>

/* DISPLAY */
static Display *g_dpy;

/* SSID */
#include <linux/wireless.h>
#define IW_INTERFACE "wlp3s0"

/* TIME */
#define TZARGENTINA "America/Buenos_Aires"

/* BRIGHTNESS */
#define BLDIR "/sys/class/backlight/intel_backlight"
#define BLMAX "max_brightness"
#define BLACT "actual_brightness"

char *
smprintf(char *fmt, ...)
{
	va_list fmtargs;
	char *ret;
	int len;

	va_start(fmtargs, fmt);
	len = vsnprintf(NULL, 0, fmt, fmtargs);
	va_end(fmtargs);

	ret = malloc(++len);
	if (ret == NULL) {
		perror("malloc");
		exit(1);
	}

	va_start(fmtargs, fmt);
	vsnprintf(ret, len, fmt, fmtargs);
	va_end(fmtargs);

	return ret;
}

void
settz(char *tzname)
{
	setenv("TZ", tzname, 1);
}

char *
mktimes(char *fmt, char *tzname)
{
	char buf[129];
	time_t tim;
	struct tm *timtm;

	settz(tzname);
	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL)
		return smprintf("");

	if (!strftime(buf, sizeof(buf)-1, fmt, timtm)) {
		fprintf(stderr, "strftime == 0\n");
		return smprintf("");
	}

	return smprintf("%s", buf);
}

char *
getssid(void)
{
    struct iwreq wreq;
	memset(&wreq, 0, sizeof(struct iwreq));
	sprintf(wreq.ifr_name, IW_INTERFACE);

	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
		exit(1);
	}

	char *id = malloc(IW_ESSID_MAX_SIZE+1);
	wreq.u.essid.pointer = id;
	wreq.u.essid.length = IW_ESSID_MAX_SIZE;
	if (ioctl(sockfd, SIOCGIWESSID, &wreq)) {
        perror("essid");
		exit(2);
	}

	return (char*)(wreq.u.essid.pointer);
}

void
setstatus(char *str)
{
	XStoreName(g_dpy, DefaultRootWindow(g_dpy), str);
	XSync(g_dpy, False);
}

char *
readfile(const char *base, const char *file)
{
	char *path, line[512+1]; /* 512 + EOF */
	FILE *fd;

	memset(line, 0, sizeof(line));

	path = smprintf("%s/%s", base, file);
	fd = fopen(path, "r");
	free(path);
	if (fd == NULL)
		return NULL;

	if (fgets(line, sizeof(line)-1, fd) == NULL)
		return NULL;
	fclose(fd);

	return smprintf("%s", line);
}

int
getbrightness(const char *bldir ,const char *blact, const char * blmax)
{
    int act = atoi(readfile(bldir, blact));
    int max = atoi(readfile(bldir, blmax));

    return (act * 100) / max;
}

int
main(void)
{
	char *status;
	char *tmar;
    char *ssid;
    int brightness;

    if (!(g_dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

	for (;;sleep(2)) {
		tmar = mktimes("%B %d (%a) %H:%M", TZARGENTINA);
        brightness = getbrightness(BLDIR, BLACT, BLMAX);
        ssid = getssid();

		status = smprintf(" B:%d%%   %s   %s ", brightness, ssid, tmar);
		setstatus(status);

        free(ssid);
		free(tmar);
		free(status);
	}

	XCloseDisplay(g_dpy);

	return 0;
}

