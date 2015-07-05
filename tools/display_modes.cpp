#include "../core/allframe.h"

using namespace allframe;

int main() {

    init();

    ALLEGRO_DISPLAY_MODE disp_data;
    for (int i = 0; i < al_get_num_display_modes(); i++) {
        al_get_display_mode(i, &disp_data);
        std::cout << disp_data.width << "x" << disp_data.height << std::endl;
    }

    close();

}
