#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "libretro.h"

#include "core/main.h"
#include "core/ee/ee.h"
#include "core/iop/iop.h"
#include "core/memmap.h"
#include "core/hle/elf.h"

#ifdef USE_BIOS_HLE
#include "core/hle/bios_hle.h"
#endif

#include "core/hw/dmac.h"

#include "common/log.h"

static struct retro_log_callback logging;
static retro_log_printf_t log_cb;
static bool use_audio_cb;
static float last_aspect;
static float last_sample_rate;

int quited = 0;

static retro_video_refresh_t video_cb;

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   va_list va;
   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}

void retro_deinit(void)
{
    delete EE::interpreter;
    delete IOP::interpreter;
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "Emotionless";
   info->library_version  = "v1 pre-alpha";
   info->need_fullpath    = false;
   info->valid_extensions = NULL; // Anything is fine, we don't care.
}

static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

const char* retro_get_system_directory(void)
{
    const char* dir;
    environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir);

    return dir ? dir : ".";
}

void retro_init(void)
{
#ifdef _WIN32
   char slash = '\\';
#else
   char slash = '/';
#endif
   unsigned i;
   unsigned color_mode = RETRO_PIXEL_FORMAT_XRGB8888;
   const char *system_dir = NULL;

   environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &color_mode);

   system_dir = retro_get_system_directory();

   EE::interpreter = new ee_interpreter();
   IOP::interpreter = new iop_interpreter();

   log_filter = error | warning | debug | verbose;

   EE::interpreter->init();
   IOP::interpreter->init();
   DMAC::init_dmac_channels();

#ifdef USE_BIOS_HLE
   HLE::init_syscalls();
#endif

   FILE* bios = fopen("ps2bios.bin","rb");
   if(bios)
   {
       fread(MemoryEE::bios,1,0x400000,bios);
       fclose(bios);
   }
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   info->timing.fps            = 60;
   info->timing.sample_rate    = 44100;
   info->geometry.base_width   = 320;
   info->geometry.base_height  = 240;
   info->geometry.max_width    = 2048;
   info->geometry.max_height   = 2048;
   info->geometry.aspect_ratio = (float)4/3;
}

static struct retro_rumble_interface rumble;

void retro_set_environment(retro_environment_t cb)
{
   bool no_content = true;
   environ_cb = cb;

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_reset(void)
{
}

void retro_run(void)
{
   static int ticks = 0;
   bool updated = false;

   input_poll_cb();

   if (quited)
      return;

   {
       for(int i = 0; i < 500000; i++)
       {
           EE::interpreter->single_step();
           DMAC::single_step();

#ifndef USE_BIOS_HLE
           if((i & 7) == 0) IOP::interpreter->single_step();
#endif
       }
   }
   /* missing: audio_cb / video_cb */

}

bool retro_load_game(const struct retro_game_info *info)
{
   int c, d;
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      log_cb(RETRO_LOG_INFO, "XRGB8888 is not supported.\n");
      return false;
   }

#ifdef USE_BIOS_HLE
   FILE* elffp = fopen(info->path,"rb");
   ELF::ELF elf;
   elf.load(elffp);
#endif

   (void)info;
   return true;
}

void retro_unload_game(void)
{
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   return retro_load_game(NULL);
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data_, size_t size)
{
   return 0;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return false;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

