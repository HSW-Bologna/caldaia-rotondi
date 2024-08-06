#include "watcher.h"
#include "model/model.h"
#include "bsp/phase_cut.h"
#include "bsp/timers.h"
#include "services/timestamp.h"


static void duty_cycle_changed(void *old_value, const void *new_value, watcher_size_t size, void *user_ptr,
                                   void *arg);
static void pid_control_changed(void *old_value, const void *new_value, watcher_size_t size, void *user_ptr,
                                   void *arg);


static watcher_t watcher = {0};


void observer_init(model_t *pmodel) {
    WATCHER_INIT_STD(&watcher, (void*)pmodel);

    WATCHER_ADD_ENTRY(&watcher, &pmodel->overridden_duty_cycle, duty_cycle_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->override_duty_cycle, duty_cycle_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->pid_output, duty_cycle_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->power, duty_cycle_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->last_communication_ts, duty_cycle_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->pid_kp, pid_control_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->pid_ki, pid_control_changed, NULL);
    WATCHER_ADD_ENTRY(&watcher, &pmodel->pid_kd, pid_control_changed, NULL);
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

    model_t *model = user_ptr;
    bsp_phase_cut_set_percentage(model_get_output_percentage(model));
}


static void pid_control_changed(void *old_value, const void *new_value, watcher_size_t size, void *user_ptr,
                                   void *arg) {
    (void)old_value;
    (void)new_value;
    (void)size;
    (void)arg;

    mut_model_t *model = user_ptr;
    model_tune_pid(model);
}
