
#include "feed.h"

feed * create_feeds()
{
    int i;
    feed f;

    feed *feeds = (feed *)malloc(sizeof(feed) * NUM_OF_FEEDS);
    if(!feeds) {
        return NULL;
    }

    for(i = 0; i < NUM_OF_FEEDS; ++i)
    {
        f = spawn_feed();

        feeds[i].id = i;
        feeds[i].x_pos = f.x_pos;
        feeds[i].y_pos = f.y_pos;
        feeds[i].radius = FEED_SPAWN_RADIUS;
        feeds[i].color = f.color;
    }

    return feeds;
}

void destroy_feeds(feed **poor)
{
    if(!poor || !*poor) {
        return;
    }

    if(*poor != NULL) {
        free(*poor);
        *poor = NULL;
    }
}

void respawn_feed(feed **feeds, int id)
{
    if(!feeds || !*feeds || id < 0 || id > NUM_OF_FEEDS) {
        return;
    }

    feed f = spawn_feed();

    (*feeds)[id].x_pos = f.x_pos;
    (*feeds)[id].y_pos = f.y_pos;
    (*feeds)[id].color = f.color;

    printf("Feed was respwaned.\n");
}

feed spawn_feed() {
    feed f;

    f.x_pos = (double) (rand() % GAME_WIDTH + 1);
    f.y_pos = (double) (rand() % GAME_HEIGHT + 1);
    f.color = rand() % (MAX_COLOR + 1);

    return f;
}
