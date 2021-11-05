#include <eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract]] hellobaru : public eosio::contract
{

private:
    struct [[eosio::table]] user
    {
        name name;
        uint16_t jumlah = 0;

        uint64_t primary_key() const { return name.value; }
    };
    using user_index = eosio::multi_index<"login"_n> user;
    //diganti sm yg diatasnya
    //typedef eosio::multi_index<"users"_n, users_struct> users_table;

public:
    hellobaru(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

    [[eosio::action]] void login(name username)
    {
        // Ensure this action is authorized by the player
        require_auth(username);

        // Create a record in the table if the player doesn't exist in our app yet
        user_index user_login(get_self(), get_self().value);
        auto iterator = user_login.find(username.value);
        if (iterator == user_login.end())
        {
            iterator = user_login.emplace(username, [&](auto &new_user)
                                          { new_user.name = username; });
        }
        else
        {
            user_login.modify(iterator, username, [&](auto &row)
                              { row.jumlah = row.jumlah + 1; });
        }
    }
};
//smart contract login
EOSIO_DISPATCH(hello, (login))
