#include "basicui.h"
#include "metronome.h"

typedef struct appdata {
    Evas_Object *win;
    Evas_Object *conform;
    Evas_Object *tempo_slider;
    Evas_Object *volume_slider;
    Evas_Object *subdivision_selector;
    Evas_Object *time_signature_label;
    Evas_Object *play_button;
    Evas_Object *stop_button;
    Metronome metronome;
    Evas_Object *label; // Add label member
} appdata_s;

static void win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
    ui_app_exit();
}

static void win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
    appdata_s *ad = data;
    /* Let window go to hide state. */
    elm_win_lower(ad->win);
}

static void tempo_slider_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
    appdata_s *ad = (appdata_s *) data;
    int tempo = static_cast<int>(elm_slider_value_get(ad->tempo_slider));
    ad->metronome.set_tempo(tempo);
}

static void volume_slider_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
    appdata_s *ad = (appdata_s *) data;
    int volume = static_cast<int>(elm_slider_value_get(ad->volume_slider));
    ad->metronome.set_volume(volume);
}

static void subdivision_selector_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
    appdata_s *ad = (appdata_s *) data;
    int subdivision = static_cast<int>(elm_spinner_value_get(ad->subdivision_selector));
    ad->metronome.set_subdivision(subdivision);
}

static void play_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    appdata_s *ad = (appdata_s *) data;
    ad->metronome.play();
}

static void stop_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    appdata_s *ad = (appdata_s *) data;
    // TODO: implement stop function
}

static void create_base_gui(appdata_s *ad)
{
    /* Window */
    /* Create and initialize elm_win.
       elm_win is mandatory to manipulate window. */
    ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
    elm_win_autodel_set(ad->win, EINA_TRUE);

    if (elm_win_wm_rotation_supported_get(ad->win)) {
        int rots[4] = { 0, 90, 180, 270 };
        elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
    }

    evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
    eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

    /* Conformant */
    /* Create and initialize elm_conformant.
       elm_conformant is mandatory for base gui to have proper size
       when indicator or virtual keypad
       /* Box Packing */
evas_object_size_hint_weight_set(volume_slider, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(volume_slider, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_box_pack_end(box, volume_slider);

evas_object_size_hint_weight_set(subdivision_selector, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(subdivision_selector, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_box_pack_end(box, subdivision_selector);

evas_object_size_hint_weight_set(ts_selector, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(ts_selector, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_box_pack_end(box, ts_selector);

evas_object_size_hint_weight_set(play_button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(play_button, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_box_pack_end(box, play_button);

evas_object_size_hint_weight_set(stop_button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(stop_button, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_box_pack_end(box, stop_button);

/* Create a Metronome object and save it in appdata */
ad->metronome = Metronome();
ad->metronome.set_tempo(static_cast<int>(elm_slider_value_get(ad->tempo_slider)));
ad->metronome.set_volume(static_cast<int>(elm_slider_value_get(ad->volume_slider)));
ad->metronome.set_subdivision(static_cast<int>(elm_spinner_value_get(ad->subdivision_selector)));

/* Set up the callbacks */
evas_object_smart_callback_add(ad->tempo_slider, "changed", tempo_slider_changed_cb, ad);
evas_object_smart_callback_add(ad->volume_slider, "changed", volume_slider_changed_cb, ad);
evas_object_smart_callback_add(ad->subdivision_selector, "changed", subdivision_selector_changed_cb, ad);
evas_object_smart_callback_add(ad->play_button, "clicked", play_button_clicked_cb, ad);
evas_object_smart_callback_add(ad->stop_button, "clicked", stop_button_clicked_cb, ad);

/* Show window */
evas_object_show(ad->win);

/* Start main loop */
elm_run();

/* Release all resources */
elm_shutdown();
}

