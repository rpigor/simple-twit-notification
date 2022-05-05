#pragma once

#include <string>

namespace Messages {
    static const std::string SESSION_COMMAND = "sessao";
    static const std::string FOLLOW_COMMAND = "seguir";
    static const std::string TWEET_COMMAND = "tweet";
    static const std::string NOTIFICATION_RESPONSE_COMMAND = "notify";

    static const std::string ASK_FOR_LEADER_COMMAND = "group";
    static const std::string LEADER_RESPONSE_COMMAND = "leader";

    static const std::string INVALID_SESSION = "sessao invalida";
    static const std::string PROFILE_NOT_FOUND = "perfil nao existe";

    static const std::string PROFILE_TO_FOLLOW_NOT_FOUND = "perfil para seguir nao existe";
    static const std::string PROFILE_CANNOT_FOLLOW_ITSELF = "perfil nao pode seguir a si mesmo";

    static const std::string TWEET_EXCEEDS_CHAR_LIMIT = "mensagem excede 128 caracteres";
}