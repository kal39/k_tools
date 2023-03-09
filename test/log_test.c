#define K_LOG_IMPLEMENTATION
#include "../k_log.h"

void cb_fn(k_LogEntry entry) {
    printf("cb: %s\n", entry.message);
}

int main(void) {
    k_LOG_I("testing info");
    k_LOG_L("testing low");
    k_LOG_M("testing medium");
    k_LOG_H("testing high");

    k_LOG_I("t n l\ne e i\ns w n\nt   e\ni   s\nn\ng");
    k_LOG_I("testing variables: %d, %f", 123, 45.6);

    k_log_set_cb_function(cb_fn);
    k_LOG_I("testing cb function");
    k_log_set_cb_function(NULL);

    k_set_max_filename_len(10);
    k_LOG_I("testing short max filename len");
}