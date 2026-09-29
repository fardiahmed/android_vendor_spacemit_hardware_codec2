/* AIDL Codec2 service for the SpacemiT VPU (amvx) serving v4l2_codec2's V4L2ComponentStore. */

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <codec2/aidl/ComponentStore.h>
#include <codec2/common/HalSelection.h>
#include <minijail.h>
#include <signal.h>

#include <v4l2_codec2/v4l2/V4L2ComponentStore.h>

static constexpr char kBaseSeccompPolicyPath[] =
        "/vendor/etc/seccomp_policy/android.hardware.media.c2-default-seccomp_policy";
static constexpr char kExtSeccompPolicyPath[] =
        "/vendor/etc/seccomp_policy/android.hardware.media.c2-extended-seccomp_policy";

static constexpr int kThreadCount = 8;

int main(int /* argc */, char** /* argv */) {
    using namespace ::aidl::android::hardware::media::c2;

    LOG(INFO) << "android.hardware.media.c2 (V4L2, K1 VPU) service starting";
    signal(SIGPIPE, SIG_IGN);
    android::SetUpMinijail(kBaseSeccompPolicyPath, kExtSeccompPolicyPath);

    if (!android::IsCodec2AidlHalSelected()) {
        // Only the AIDL store is declared in this service's VINTF fragment.
        LOG(ERROR) << "AIDL Codec2 HAL not selected (media.c2.hal.selection); not registering";
        return 1;
    }

    ABinderProcess_setThreadPoolMaxThreadCount(kThreadCount);
    ABinderProcess_startThreadPool();

    std::shared_ptr<C2ComponentStore> v4l2Store = android::V4L2ComponentStore::Create();
    if (v4l2Store == nullptr) {
        LOG(ERROR) << "Cannot create V4L2ComponentStore";
        return 1;
    }
    std::shared_ptr<IComponentStore> store =
            ::ndk::SharedRefBase::make<utils::ComponentStore>(v4l2Store);

    const std::string serviceName = std::string(IComponentStore::descriptor) + "/default";
    binder_exception_t ex = AServiceManager_addService(store->asBinder().get(),
                                                       serviceName.c_str());
    if (ex != EX_NONE) {
        LOG(ERROR) << "Cannot register " << serviceName;
        return 1;
    }
    LOG(INFO) << "Registered " << serviceName;

    ABinderProcess_joinThreadPool();
    return 0;
}
