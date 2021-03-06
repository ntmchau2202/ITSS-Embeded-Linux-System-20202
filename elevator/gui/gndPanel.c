#include "../signals/signals.h"
#include <gtk/gtk.h>

pid_t *pid_list;
int current_floor_number = 1;
int fifoFd;
GtkWidget *window;
GtkWidget *button2;
GtkWidget *button3;
GtkWidget *button4;
GtkWidget *button5;
GtkWidget *close_btn;
GtkWidget *button_box;
GtkWidget *main_box;
GtkWidget *up_down_btn;
GtkWidget *current_floor_btn;
char window_title[10], current_floor[10], current_label[5];

static void
floor2Handler(GtkWidget *widget,
      gpointer data)
{
    int tang = 2;
    if (strcmp(gtk_widget_get_name(button2), "red_btn") != 0)
    {
        gtk_widget_set_name(button2, "red_btn");
        sendSignal(pid_list[LIFT_MNG], F2_UP);
        write(fifoFd, &tang, sizeof(int));
    }
}
static void
floor3Handler(GtkWidget *widget,
      gpointer data)
{
    int tang = 3;
    if (strcmp(gtk_widget_get_name(button3), "red_btn") != 0)
    {
        gtk_widget_set_name(button3, "red_btn");
        sendSignal(pid_list[LIFT_MNG], F3_UP);
        write(fifoFd, &tang, sizeof(int));
    }
}
static void
floor4Handler(GtkWidget *widget,
      gpointer data)
{
    int tang = 4;
    if (strcmp(gtk_widget_get_name(button4), "red_btn") != 0)
    {
        gtk_widget_set_name(button4, "red_btn");
        sendSignal(pid_list[LIFT_MNG], F4_UP);
        write(fifoFd, &tang, sizeof(int));
    }
}
static void
floor5Handler(GtkWidget *widget,
      gpointer data)
{
    int tang = 5;
    if (strcmp(gtk_widget_get_name(button5), "red_btn") != 0)
    {
        gtk_widget_set_name(button5, "red_btn");
        sendSignal(pid_list[LIFT_MNG], F5_UP);
        write(fifoFd, &tang, sizeof(int));
    }
}

GdkPixbuf *create_pixbuf(const gchar *filename)
{

    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);

    if (!pixbuf)
    {

        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}
static void
activate(GtkApplication *app,
         gpointer user_data)
{
    // <-------------  For add stylesheet.css
    GtkCssProvider *Provider = gtk_css_provider_new();
    GdkDisplay *Display = gdk_display_get_default();
    GdkScreen *Screen = gdk_display_get_default_screen(Display);

    gtk_style_context_add_provider_for_screen(Screen, GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "../resources/stylesheet.css", NULL);
    // End add stylesheet.css ---------------->

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), window_title);
    gtk_window_set_default_size(GTK_WINDOW(window), 150, 200);
    gtk_window_move(GTK_WINDOW(window), 150, 620);
    gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("../resources/icon"));
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_widget_set_name(window, "window");

    //   Add Vbox
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_set_name(main_box, "first_floor_window");

    button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_add(GTK_CONTAINER(main_box), button_box);
    gtk_widget_set_name(button_box, "first_floor_button_box");

    up_down_btn = gtk_button_new_with_label("FLOOR I");
    gtk_widget_set_name(up_down_btn, "info_btn");
    gtk_widget_set_sensitive(up_down_btn, FALSE);
    gtk_container_add(GTK_CONTAINER(button_box), up_down_btn);

    current_floor_btn = gtk_button_new_with_label("I");
    gtk_widget_set_name(current_floor_btn, "unready_btn");

    gtk_widget_set_sensitive(current_floor_btn, FALSE);
    gtk_container_add(GTK_CONTAINER(button_box), current_floor_btn);
    // button 2
    button2 = gtk_button_new_with_label("II");
    g_signal_connect(button2, "clicked", G_CALLBACK(floor2Handler), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button2);
    // button3
    button3 = gtk_button_new_with_label("III");
    g_signal_connect(button3, "clicked", G_CALLBACK(floor3Handler), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button3);
    // button4
    button4 = gtk_button_new_with_label("IV");

    g_signal_connect(button4, "clicked", G_CALLBACK(floor4Handler), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button4);
    // button5
    button5 = gtk_button_new_with_label("V");
    g_signal_connect(button5, "clicked", G_CALLBACK(floor5Handler), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button5);

    // show all widget
    gtk_widget_show_all(window);
}
void currentFloorChange(int sigNo)
{
    gtk_widget_set_name(current_floor_btn, "unready_btn");
    current_floor_number = sigNo - SIGRTMIN;

    if(sigNo == F1_ARRIVAL) {
        gtk_button_set_label(GTK_BUTTON(current_floor_btn), "1");
    } else if (sigNo == F2_ARRIVAL) {
    gtk_button_set_label(GTK_BUTTON(current_floor_btn), "2");
    } else if (sigNo == F3_ARRIVAL) {
        gtk_button_set_label(GTK_BUTTON(current_floor_btn), "3");
    } else if (sigNo == F4_ARRIVAL) {
        gtk_button_set_label(GTK_BUTTON(current_floor_btn), "4");
    } else if (sigNo == F5_ARRIVAL) {
        gtk_button_set_label(GTK_BUTTON(current_floor_btn), "5");
    }
}
void finishMove()
{
    int tang;
    int res = read(fifoFd, &tang, sizeof(int));
    if (res > 0)
        switch (tang)
        {
        case 2:
            gtk_button_set_label(GTK_BUTTON(current_floor_btn), "2");
            gtk_widget_set_name(button2, "default_btn");
            break;
        case 3:
            gtk_button_set_label(GTK_BUTTON(current_floor_btn), "3");
            gtk_widget_set_name(button3, "default_btn");
            break;
        case 4:
            gtk_button_set_label(GTK_BUTTON(current_floor_btn), "4");
            gtk_widget_set_name(button4, "default_btn");
            break;
        case 5:
            gtk_button_set_label(GTK_BUTTON(current_floor_btn), "5");
            gtk_widget_set_name(button5, "default_btn");
            break;
        default:
            break;
        }
}
void using()
{
    gtk_button_set_label(GTK_BUTTON(current_floor_btn), "");
    gtk_widget_set_name(current_floor_btn, "ready_btn");
}
void finishMoveAndUsing()
{
    //printf("Tang 1 get arrives notification\n");

    int tang;
    gtk_button_set_label(GTK_BUTTON(current_floor_btn), "");
    if (read(fifoFd, &tang, sizeof(int)) > 0)
        switch (tang)
        {
        case 2:
            gtk_widget_set_name(current_floor_btn, "ready_btn");

            gtk_widget_set_name(button2, "default_btn");
            break;
        case 3:
            gtk_widget_set_name(current_floor_btn, "ready_btn");

            gtk_widget_set_name(button3, "default_btn");
            break;
        case 4:
            gtk_widget_set_name(current_floor_btn, "ready_btn");

            gtk_widget_set_name(button4, "default_btn");
            break;
        case 5:
            gtk_widget_set_name(current_floor_btn, "ready_btn");

            gtk_widget_set_name(button5, "default_btn");
            break;
        default:
            break;
        }
}
int main(int argc, char *argv[])
{

    registerArrivalSignals(currentFloorChange);
    registerFinishedSignals(finishMove);
    registerUsingSignals(using);
    registerFinishedUsingSignals(finishMoveAndUsing);

    pid_list = updatePID(FLR_PNL1);
    setpgid(pid_list[FLR_PNL1], pid_list[LIFT_MNG]);
    printf("ope1_process_id %d\n", pid_list[FLR_PNL1]);
    
    //Make FIFO file
    remove(GND_FIFO_FILE);
    if (mkfifo(GND_FIFO_FILE, 0666) == -1)
    {
        perror("mkfifo");
        exit(1);
    }
    //Open for writing only
    if ((fifoFd = open(GND_FIFO_FILE, O_RDWR | O_NONBLOCK)) == -1)
    {
        perror("fifofile open");
        exit(1);
    }
    GtkApplication *app;
    int status;
    strcpy(window_title, "1st Floor");
    app = gtk_application_new("org.gtk.tang1", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    close(fifoFd);
    unlink(GND_FIFO_FILE);
    return status;
}