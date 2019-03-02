# Eink Frame

## Compile Instructions

To make this code compile for the ESP32 dev board, you'll need to make the following changes in the `lib/gfx/ugfx` submodule:
1. in `lib/gfx/ugfx/src/gos/gos_freertos.h` exchange
```c
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
#include "task.h"
```

by

```c
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
```

2. In `lib/gfx/ugfx/src/gdisp/mcufont/mf_config.h` exchange `#define MF_FONT_FILE_NAME "src/gdisp/fonts/fonts.h"` by `#define MF_FONT_FILE_NAME "../fonts/fonts.h"`
3. In `lib/gfx/ugfx/src/gdisp/fonts/DejaVuSans[16|32|32_aa].c` exchange `#include "mf_rlefont.h"` by `#include "../mcufont/mf_rlefont.h"`
