#define K_LOG_DEBUG
#define K_LOG_IMPLEMENTATION
#include "../k_log.h"

int main(void) {
	k_enable_debug_logs();
	k_log_to_file("test/out/k_log_output");

	k_note("Testing msg");
	k_debug("Testing dbg");
	k_error("Testing err");

	k_note("t n l\ne e i\ns w n\nt   e\ni   s\nn\ng");
	k_note("Testing variables: %d, %f", 123, 45.6);

	k_set_max_filename_length(10);

	k_stop_file_log();

	k_panic(1, "Testing panic");
}