#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define ICON_SIZE 64
#define PADDING 8
#define N_FRAMES 24
#define ICON_PATH_TEMPLATE "/tmp/swap_icon_%d.png"

static void save_icon_to_file(GdkPixbuf *pixbuf, const char *filepath);
static GdkPixbuf *create_icon(const char *color, double phase);
static bool check_swap(char *swap_used);
static void *update_icon(void *arg);
static void quit_action(GtkMenuItem *item, gpointer user_data);

typedef struct {
    AppIndicator *indicator;
    volatile bool running;
} MonitorData;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    MonitorData data;
    data.running = true;

    // Create AppIndicator
    data.indicator = app_indicator_new(
        "swap-monitor", "process-working-symbolic",
        APP_INDICATOR_CATEGORY_SYSTEM_SERVICES);
    app_indicator_set_status(data.indicator, APP_INDICATOR_STATUS_ACTIVE);

    // Create menu
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *title_item = gtk_menu_item_new_with_label("Swap Monitor");
    gtk_widget_set_sensitive(title_item, FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), title_item);

    GtkWidget *quit_item = gtk_menu_item_new_with_label("Quit");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit_item);

    g_signal_connect(quit_item, "activate", G_CALLBACK(quit_action), &data);
    gtk_widget_show_all(menu);

    app_indicator_set_menu(data.indicator, GTK_MENU(menu));

    // Start updater thread
    pthread_t updater_thread;
    pthread_create(&updater_thread, NULL, update_icon, &data);

    gtk_main();

    data.running = false;
    pthread_join(updater_thread, NULL);

    return 0;
}

static void save_icon_to_file(GdkPixbuf *pixbuf, const char *filepath) {
    GError *error = NULL;
    gdk_pixbuf_save(pixbuf, filepath, "png", &error, NULL);
    if (error) {
        g_printerr("Error saving icon to file: %s\n", error->message);
        g_error_free(error);
    }
}

static GdkPixbuf *create_icon(const char *color, double phase) {
    int red, green, blue;
    sscanf(color, "#%02x%02x%02x", &red, &green, &blue);

    red = (int)(red * pow(cos(phase * M_PI), 2));
    green = (int)(green * pow(cos(phase * M_PI), 2));
    blue = (int)(blue * pow(cos(phase * M_PI), 2));

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ICON_SIZE, ICON_SIZE);
    cairo_t *cr = cairo_create(surface);

    cairo_set_source_rgb(cr, red / 255.0, green / 255.0, blue / 255.0);
    cairo_arc(cr, ICON_SIZE / 2, ICON_SIZE / 2, (ICON_SIZE - PADDING * 2) / 2, 0, 2 * M_PI);
    cairo_fill(cr);

    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, ICON_SIZE, ICON_SIZE);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return pixbuf;
}

static bool check_swap(char *swap_used) {
    FILE *fp = popen("free -h | awk '/Swap/ {print $3}'", "r");
    if (!fp) return false;

    fgets(swap_used, 32, fp);
    pclose(fp);

    // Remove newline
    swap_used[strcspn(swap_used, "\n")] = '\0';

    return strcmp(swap_used, "0B") != 0;
}

static void *update_icon(void *arg) {
    MonitorData *data = (MonitorData *)arg;

    GdkPixbuf *red_icons[N_FRAMES];
    char red_icon_paths[N_FRAMES][64];
    for (int i = 0; i < N_FRAMES; ++i) {
        red_icons[i] = create_icon("#FF0000", (double)i / N_FRAMES);
        snprintf(red_icon_paths[i], sizeof(red_icon_paths[i]), ICON_PATH_TEMPLATE, i);
        save_icon_to_file(red_icons[i], red_icon_paths[i]);
    }

    GdkPixbuf *green_icon = create_icon("#00FF00", 0);
    char green_icon_paths[2][64];
    snprintf(green_icon_paths[0], sizeof(green_icon_paths[0]), ICON_PATH_TEMPLATE, N_FRAMES);
    snprintf(green_icon_paths[1], sizeof(green_icon_paths[1]), ICON_PATH_TEMPLATE, N_FRAMES + 1);
    save_icon_to_file(green_icon, green_icon_paths[0]);
    save_icon_to_file(green_icon, green_icon_paths[1]);

    int frame = 0;
    char swap_used[32];
    bool swap_active;

    char status[128];

    while (data->running) {
        swap_active = check_swap(swap_used);

        if (swap_active) {
            app_indicator_set_icon(data->indicator, red_icon_paths[frame]);
            snprintf(status, sizeof(status), "Swap used: %s", swap_used);
            app_indicator_set_label(data->indicator, status, NULL);
        } else {
            app_indicator_set_icon(data->indicator, green_icon_paths[frame % 2]);
            app_indicator_set_label(data->indicator, "Swap not used", NULL);
        }

        frame = (frame + 1) % N_FRAMES;
        usleep(1000000 / N_FRAMES);
    }

    for (int i = 0; i < N_FRAMES; ++i) {
        g_object_unref(red_icons[i]);
    }
    g_object_unref(green_icon);

    return NULL;
}

static void quit_action(GtkMenuItem *item, gpointer user_data) {
    MonitorData *data = (MonitorData *)user_data;
    data->running = false;
    gtk_main_quit();
}
