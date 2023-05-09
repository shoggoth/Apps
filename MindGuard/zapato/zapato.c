//
//  zapato.c
//  libzapato.a
//
//  Created by Richard Henry on 17/06/2021.
//

#include <stdio.h>

void test_zapato(void) {
    printf("test_zapato called\n");
}

#ifdef NAGGERS

// BIORHYTHM
void create_biorhythm(GtkWidget *widget)
{
	int i, ii, w, h, y, x2, y2, md, days, new;
	float r, x, bio_len[3] = {23, 28, 33}, pi2;

	pi2 = 3.14159265358979323846 * 2;

	/*
	23 = Physical (red)
	28 = Emotional (green)
	33 = Intellectual (blue)
	There is also a hypothesized 38 day Intuitional biorhythm,
	but it hasn't been full tested by the scientific community
	so we will forgo its inclusion until its existence is put
	on a more empirically sound footing.
	*/

	days = days_since();

	w = widget->allocation.width;
	h = widget->allocation.height;

	if (pixmap_bio)
		gdk_pixmap_unref(pixmap_bio);

	pixmap_bio = gdk_pixmap_new (widget->window, w, h, -1);

	gdk_draw_rectangle (pixmap_bio, widget->style->bg_gc[GTK_WIDGET_STATE(widget)],
		TRUE, 0, 0, w, h);

	/* axes and ticks */
	gdk_draw_line (pixmap_bio, widget->style->black_gc, 0, (h/2), w, (h/2));
	gdk_draw_line (pixmap_bio, widget->style->black_gc, (w/2), 0, (w/2), h);
	for ( i = 1 ; i < 14; i++)
		{
		x = (float) w / 28;
		r = i;
	    	gdk_draw_line (pixmap_bio, widget->style->black_gc,
			(w/2)+(x * r), (h/2) - 3,
			(w/2)+(x * r), (h/2) + 3);
	    	gdk_draw_line (pixmap_bio, widget->style->black_gc,
			(w/2)-(x * r), (h/2) - 3,
			(w/2)-(x * r), (h/2) + 3);
		}

 	/* bio-sines */
	for ( i = 0 ; i < 3; i++)
		{
		x2 = bio_len[i];
		md = (days - 14) % x2;
		x2 = 0; new = TRUE;
		x = (md / bio_len[i]) * pi2;
		for ( ii = 0 ; ii < w + 1; ii++)
			{
			r = (ii / (float) w) * pi2 * (28 / bio_len[i]) + x;
			y = -1 * sin(r) * ((h/2) - 2) + (h/2);
			if (new)
				{
				y2 = y; new = FALSE;
				}
		    	gdk_draw_line (pixmap_bio, bio_gc[i], x2, y2, ii, y);
			x2 = ii; y2 = y;
			}
		}

	return;
}
void draw_biorhythm(GtkWidget *widget, GdkEventExpose *event)
{
	gdk_draw_pixmap(widget->window,
		widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		pixmap_bio,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);
	return;
}
void update_biorhythm()
{
	create_biorhythm(draw_bio);
	gdk_draw_pixmap(draw_bio->window,
		draw_bio->style->fg_gc[GTK_WIDGET_STATE(draw_bio)],
		pixmap_bio, 0, 0, 0, 0,
		draw_bio->allocation.width, draw_bio->allocation.height);
	return;
}


/**** EPONYMOLOG ****/
int days_since()
{
    int days, day, month, year;
    time_t now;
    struct tm *tm;

    now = time(0);
    tm = localtime(&now);

    day = tm->tm_mday;
    month = tm->tm_mon + 1;
    year = tm->tm_year;
    if (year >= 90)
        year = year + 1900;
    else
        year = year + 2000;

    /*
    MindGuard is Y2K compliant -- but not Y209DA compliant.
    This will be fixed in MindGuard version 0.0.2.0, which
    should be done before 2090. Until then, please don't run
    MindGuard more than ten years ago.
    */

    days = ratadie(day, month, year) - ratadie(bday, bmonth, byear);

    return days;
}

void update_username_eponymolog()
{
	strcpy (user_name_data, gtk_entry_get_text(GTK_ENTRY(entry_epo)));
	update_eponymolog();

	if (strlen(user_name) == 0)
		{
		state_epo = FALSE;
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check_epo), FALSE);
		}

	/* MindGuard cares about your wellbeingness. */
	if (eponymolog() == 13)
		popup_dialog("Your eponymolog is the unlucky number 13.\n"
			"MindGuard suggests that you consult your\n"
			"local chapter of The Society of Kabalarians\n"
			"for advice on coping with this.", "Ok",
			NULL, NULL, NULL, DIALOG_BEEP | DIALOG_CENTER);
	return;
}
void update_eponymolog()
{
	char text[4];
	sprintf (text, "%i", eponymolog());
	gtk_label_set_text (GTK_LABEL(label_epo), text);
	return;
}
int eponymolog(void)
{
	/*
	Ancient Numerological Algorithm as used by the Pythagorians. Later
	rediscovered and developed for eponymological use by the noted
	19th Century logistician and crazed mystic, George Boole.
	*/

	int i;
	char value;

	value = 0;
	for (i = 0 ; user_name[i] > 0; i++)
		value = value ^ user_name[i];

	/*
	Mystical Exclusive Or. Note: operator symbol forms a partial
	isosceles triangle. This is not a coincidence!
	*/

	return value;
}

#endif
