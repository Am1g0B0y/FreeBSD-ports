--- src/3rdparty/chromium/content/renderer/render_thread_impl.cc.orig	2018-11-13 18:25:11 UTC
+++ src/3rdparty/chromium/content/renderer/render_thread_impl.cc
@@ -199,12 +199,21 @@
 #include "mojo/public/cpp/bindings/message_dumper.h"
 #endif
 
+#if !defined(OS_BSD)
 #if defined(OS_MACOSX)
 #include <malloc/malloc.h>
 #else
 #include <malloc.h>
 #endif
+#endif
 
+#if defined(OS_BSD)
+#include <stddef.h>
+#include <stdint.h>
+#include <sys/param.h>
+#include <sys/sysctl.h>
+#endif
+
 using base::ThreadRestrictions;
 using blink::WebDocument;
 using blink::WebFrame;
@@ -980,7 +989,7 @@ void RenderThreadImpl::Init(
   DCHECK(parsed_num_raster_threads) << string_value;
   DCHECK_GT(num_raster_threads, 0);
 
-#if defined(OS_LINUX)
+#if defined(OS_LINUX) || defined(OS_BSD)
   categorized_worker_pool_->SetBackgroundingCallback(
       main_thread_scheduler_->DefaultTaskRunner(),
       base::BindOnce(
@@ -1021,7 +1030,7 @@ void RenderThreadImpl::Init(
   GetConnector()->BindInterface(mojom::kBrowserServiceName,
                                 mojo::MakeRequest(&storage_partition_service_));
 
-#if defined(OS_LINUX)
+#if defined(OS_LINUX) || defined(OS_BSD)
   render_message_filter()->SetThreadPriority(
       ChildProcess::current()->io_thread_id(), base::ThreadPriority::DISPLAY);
 #endif
@@ -1227,7 +1236,7 @@ void RenderThreadImpl::InitializeCompositorThread() {
                      false));
   GetContentClient()->renderer()->PostCompositorThreadCreated(
       compositor_task_runner_.get());
-#if defined(OS_LINUX)
+#if defined(OS_LINUX) || defined(OS_BSD)
   render_message_filter()->SetThreadPriority(compositor_thread_->ThreadId(),
                                              base::ThreadPriority::DISPLAY);
 #endif
@@ -1451,11 +1460,11 @@ media::GpuVideoAcceleratorFactories* RenderThreadImpl:
        gpu::kGpuFeatureStatusEnabled);
   const bool enable_gpu_memory_buffers =
       !is_gpu_compositing_disabled_ &&
-#if defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_WIN)
+#if defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_WIN) || defined(OS_BSD)
       !cmd_line->HasSwitch(switches::kDisableGpuMemoryBufferVideoFrames);
 #else
       cmd_line->HasSwitch(switches::kEnableGpuMemoryBufferVideoFrames);
-#endif  // defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_WIN)
+#endif  // defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_WIN) || defined(OS_BSD)
   const bool enable_media_stream_gpu_memory_buffers =
       enable_gpu_memory_buffers &&
       base::FeatureList::IsEnabled(
