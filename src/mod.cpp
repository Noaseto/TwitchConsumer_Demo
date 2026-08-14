#include "mods/hook.hpp"
#include "mods/service.hpp"
#include "mods/svc/hook.h"
#include "mods/svc/log.h"

#include "twitchLoaderService.h"
#include <nlohmann/json.hpp>

// Game includes
#include "d/d_item_data.h"
#include "d/actor/d_a_alink.h"
#include "f_op/f_op_actor_mng.h"

DEFINE_MOD();

IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(HookService, svc_hook);
IMPORT_SERVICE(TwitchLoaderService, svc_twitch);

static ModResult handle_twitch_event(const TwitchEvent& event) {
    auto linkActor = daAlink_getAlinkActorClass();

    auto linkPos = &linkActor->current.pos;
    auto roomNo = &linkActor->current.roomNo;

    char buf[32];
    snprintf(buf, sizeof(buf), "roomNo %d", *roomNo);
    svc_log->info(mod_ctx, buf);

    nlohmann::json j = nlohmann::json::parse(event.data);
    std::string type = j["metadata"]["message_type"];
    switch (event.type) {
    case TwitchEventType::ChatMessage: {
        std::string message = j["payload"]["event"]["message"]["text"];
         if (message == "rupee") {
             for (int i=0; i<20; i++) {
                 fopAcM_createItem(linkPos, dItemNo_GREEN_RUPEE_e, -1, *roomNo, NULL, NULL, 0);
             }
         }else {
            fopAcM_createItem(linkPos, dItemNo_GREEN_RUPEE_e, -1, *roomNo, NULL, NULL, 0);
        }
        break;
    }
    case TwitchEventType::Follow: {
        fopAcM_createItem(linkPos, dItemNo_HEART_e, -1, *roomNo, NULL, NULL, 0);
        break;
    }
    case TwitchEventType::Subscribe: {
        fopAcM_createItem(linkPos, dItemNo_BLUE_RUPEE_e, -1, *roomNo, NULL, NULL, 0);
        break;
    }
    case TwitchEventType::SubGift:
    case TwitchEventType::Cheer:
    case TwitchEventType::Unknown:
    case TwitchEventType::SessionWelcome: {
        // this event is only for Twitch loader communication with twitch
        svc_log->debug(mod_ctx, ("Event" + type + " not handled").c_str());
        break;
    }
    case TwitchEventType::TwitchEventError:
    default:
        svc_log->error(mod_ctx, ("Event "+type+" is error").c_str());
        return MOD_ERROR;
    }
    return MOD_OK;
}

MOD_EXPORT ModResult mod_initialize(ModError*) {
    svc_log->info(mod_ctx, "TwitchService demo mod initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    const TwitchEvent* events = nullptr;
    uint32_t eventCount = 0;

    ModResult result = svc_twitch->get_events(mod_ctx, &events, &eventCount);
    if (result != MOD_OK) {
        return result;
    }

    // computes what needs to be done for each event since last tick
    for (uint32_t eventIndex = 0; eventIndex < eventCount; ++eventIndex) {
        handle_twitch_event(events[eventIndex]);
    }

    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}