#pragma once

#include <string>

namespace Messages {
    const std::string SESSION_COMMAND = "sessao";
    const std::string FOLLOW_COMMAND = "seguir";
    const std::string TWEET_COMMAND = "tweet";
    const std::string NOTIFICATION_RESPONSE_COMMAND = "notify";

    const std::string ASK_FOR_LEADER_COMMAND = "group";
    const std::string LEADER_RESPONSE_COMMAND = "leader";

    const std::string INVALID_SESSION = "sessao invalida";
    const std::string PROFILE_NOT_FOUND = "perfil nao existe";

    const std::string PROFILE_TO_FOLLOW_NOT_FOUND = "perfil para seguir nao existe";
    const std::string PROFILE_CANNOT_FOLLOW_ITSELF = "perfil nao pode seguir a si mesmo";

    const std::string TWEET_EXCEEDS_CHAR_LIMIT = "mensagem excede limite";
}