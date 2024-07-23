#include "watcher.h"
#include "model/model.h"
#include "bsp/phase_cut.h"
#include "services/timestamp.h"


static void duty_cycle_changed(void *old_value, const void *new_value, watcher_size_t size, void *user_ptr,
                                   void *arg);


static watcher_t watcher = {0};


void observer_init(model_t *pmodel) {
    WATCHER_INIT_STD(&watcher, (void*)pmodel);

    WATCHER_ADD_ENTRY(&watcher, &pmodel->overridden_duty_cycle, duty_cycle_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->override_duty_cycle, duty_cycle_changed, NULL);
}


void observer_manage(model_t *pmodel) {
    (void)pmodel;
    watcher_watch(&watcher, bsp_timers_get_millis());
}


static void duty_cycle_changed(void *old_value, const void *new_value, watcher_size_t size, void *user_ptr,
                                   void *arg) {
    (void)old_value;
    (void)new_value;
    (void)size;
    (void)arg;

    model_t *pmodel = user_ptr;

    if (pmodel->override_duty_cycle) {
        bsp_phase_cut_set_percentage(pmodel->overridden_duty_cycle);
    } else {
        bsp_phase_cut_set_percentage(0);
    }
}
