#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
using namespace eosio;

CONTRACT rexapis : public contract {
    public:
        using contract::contract;

        struct [[eosio::table]] rex_pool {
           uint8_t    version = 0;
           asset      total_lent;
           asset      total_unlent;
           asset      total_rent;
           asset      total_lendable;
           asset      total_rex;
           asset      namebid_proceeds;
           uint64_t   loan_num = 0;

           uint64_t primary_key()const { return 0; }
        };

        typedef eosio::multi_index< "rexpool"_n, rex_pool > rex_pool_table;

        struct pair_time_point_sec_int64 {
			time_point_sec first;
			int64_t        second;
		};

        struct [[eosio::table]] rex_balance {
           uint8_t version = 0;
           name    owner;
           asset   vote_stake;
           asset   rex_balance;
           int64_t matured_rex = 0;
           std::vector<pair_time_point_sec_int64> rex_maturities; /// REX daily maturity buckets

           uint64_t primary_key()const { return owner.value; }
        };

        typedef eosio::multi_index< "rexbal"_n, rex_balance > rex_balance_table;


        [[eosio::action, eosio::read_only]]
        asset convert( asset rex_or_vaulta ){
            rex_pool_table rex_pool("eosio"_n, "eosio"_n.value);
            auto pool = rex_pool.begin();

            if (rex_or_vaulta.symbol == symbol("A", 4)) {
                // This is convertEosToRex(eos)
                auto S0 = (double)pool->total_lendable.amount;
                auto S1 = S0 + (double)rex_or_vaulta.amount;
                auto R0 = (double)pool->total_rex.amount;
                auto R1 = (S1 * R0) / S0;
                return asset(R1 - R0, symbol("REX", 4));

            } else if (rex_or_vaulta.symbol == symbol("REX", 4)) {
                // This is convertRexToEos(rex)
                auto S0 = (double)pool->total_lendable.amount;
                auto R0 = (double)pool->total_rex.amount;
                auto R1 = R0 + (double)rex_or_vaulta.amount;
                auto S1 = (S0 * R1) / R0;
                return asset(S1 - S0, symbol("A", 4));
            } else {
                check(false, "Unsupported token for conversion");
            }
        }

        [[eosio::action, eosio::read_only]]
        double getapy() {
            rex_pool_table rex_pool("eosio"_n, "eosio"_n.value);
            auto pool = rex_pool.begin();
            check(pool != rex_pool.end(), "REX pool not found");

            double annualReward = 31250000.0;
            double totalStaked = static_cast<double>(pool->total_lendable.amount) / 10000.0;
            double apy = (annualReward / totalStaked) * 100.0;
            return round(apy * 100.0) / 100.0;
        }

        [[eosio::action, eosio::read_only]]
		asset getrewards(name account, bool mature_only = false) {
			rex_balance_table rex_balances("eosio"_n, "eosio"_n.value);
			auto balance_itr = rex_balances.find(account.value);
			if(balance_itr == rex_balances.end()){
				return asset(0, symbol("A", 4));
			}

			if(mature_only){
				int64_t total_rex = 0;
				auto now = current_time_point().sec_since_epoch();
				for(const auto& maturity : balance_itr->rex_maturities) {
					if(maturity.first.sec_since_epoch() <= now) {
						total_rex += maturity.second;
					}
				}
				return convert(asset(total_rex, symbol("REX", 4)));
			}

			return convert(balance_itr->rex_balance);
		}

};
