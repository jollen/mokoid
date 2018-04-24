#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <utils/IPCThreadState.h>
#include <utils/ProcessState.h>
#include <utils/IServiceManager.h>
#include <utils/Log.h>

#include <ui/SurfaceComposerClient.h>

using namespace android;

#define RGBA8888(r, g, b, a)	\
	((((r) & 0xff) <<  0) |	\
	 (((g) & 0xff) <<  8) |	\
	 (((b) & 0xff) << 16) |	\
	 (((a) & 0xff) << 24))

static void draw(char *buf, int w, int h)
{
	int x, y;
	int stride = w * 4; // 4 = depth / 8

	for (y = 0; y < h; y++) {
		char *row = buf + stride * y;

		for (x = 0; x < w; x++) {
			int r, g, b, a;

			r = 0xff;
			g = 0x00;
			b = 0x00;
			a = 0xff;

			((uint32_t *) row)[x] = RGBA8888(r, g, b, a);
		}
	}
}

int createSurface(int w, int h)
{
	Surface::SurfaceInfo info;
	char *rawBuffer;
	int depth = 32;

	sp<SurfaceComposerClient> surfaceflinger_client;
	sp<Surface> surface;

	surfaceflinger_client = new SurfaceComposerClient();

	// Create a new surface.
	surface = surfaceflinger_client->createSurface(getpid(), 0, w, h, depth, 0);

	// Control
	surface->setPosition(0, 0);
	surface->setLayer(INT_MAX);

	// Get raw buffer of Surface.
	surface->lock(&info);
    	rawBuffer = (char *)info.bits;

	// Draw a red rect
	draw(rawBuffer, 10, 10);

	// Unlock
	surface->unlockAndPost();

	return 0;
}

int main(int argc, char **argv)
{
    LOGI("MokoidSurface is started.");

    //ProcessState::self()->startThreadPool();

    createSurface(100, 100);

    //IPCThreadState::self()->joinThreadPool();

    return 0;
}
