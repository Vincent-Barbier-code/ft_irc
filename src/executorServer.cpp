#include "server.hpp"

void Server::_execute(Client &client, Message const &msg)
{

    int client_fd = client.getFd();
    std::string const &cmd = msg.getCmd();
    std::vector<std::string> paramsV = msg.getParamsValues();
    try
    {
        if (msg.getErr())
            clerr(msg.getErr());
        else if (cmd == "PING")
            _sendMsgToClient(client, "PONG " + paramsV[0]);
        else if (cmd == "PASS")
           client.pass(paramsV[0], getPass());
        else if (!client.isAuth())
            clerr(ERR_NOTREGISTERED);
        else if (cmd == "NICK")
            _nick(client_fd, paramsV[0]);
        else if (!client.isNicked())
            clerr(ERR_NONICKNAMEGIVEN);
        /* - - - - - - - - - - - - - After PASS/NICK - - - - - - - */
        else if (cmd == "USER")
            _user(client, paramsV);
        else if (!client.isRegistered())
            clerr(ERR_NOTREGISTERED);
        /* - - - - - - - - - - - - - After USER registration - - - */
        else if (cmd == "QUIT")
            _quit(client, paramsV.size() == 1 ? paramsV[0] : "");
        else if (cmd == "JOIN")
            _join(client_fd, paramsV[0], paramsV[1]);
        else if (cmd == "LIST")
            _list(client);
        else if (cmd == "PRIVMSG")
            _sendPrivateMsg(client, paramsV[0], paramsV[1]);
        else if (cmd == "PART")
            _part(client_fd, paramsV[0]);
        else if (cmd == "TOPIC")
            _topic(client_fd, paramsV[0], paramsV[1]);
        else if (cmd == "KICK")
            _kick(client_fd, paramsV[0], paramsV[1], paramsV[2]);
        else if (cmd == "INVITE")
            _invite(client_fd, paramsV[0], paramsV[1]);
        else if (cmd == "MODE")
            mode(paramsV[0], paramsV[1].size() == 2 ? paramsV[1] : "", paramsV.size() == 3 ? paramsV[2] : "", client);
    }
    catch (Client::ClientException const &e) {
        if (e.getCode() == INVALID_CMD)
            std::cerr << RED "Invalid command: " PURPLE << msg.getCmd() << WHITE << std::endl;
        else if (e.getCode() == INVALID_CHARACTER)
            std::cerr << RED "Invalid character: non printable or leading/trailing spaces/tab...  : |" PURPLE << msg.getRawWPrefix() << RED "|" WHITE << std::endl;
        else
            _sendNumericReply(e.getCode(), client);
    }
    catch(const std::exception &e) {
        std::cerr << RED "Erreur non géré: what():" << e.what() << ", Parsing de l'erreur: " << msg << WHITE << std::endl;
    }
}