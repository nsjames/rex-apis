import { APIClient, Chains } from "@wharfkit/session"
import { ContractKit } from "@wharfkit/contract"

(async () => {
    const client = new APIClient({ url: Chains.Vaulta.url });
    const kit = new ContractKit({ client });
    const contract = await kit.load('userexapisv1');

    {
        const vaultaToRex = await contract.readonly('convert', { rex_or_vaulta: '100.0000 A' });
        const rexToVaulta = await contract.readonly('convert', { rex_or_vaulta: '100.0000 REX' });

        console.log('REX to Vaulta:', vaultaToRex.toString());
        console.log('Vaulta to REX:', rexToVaulta.toString());
    }


    {
        const result = await contract.readonly('getapy');
        console.log('APY:', result.toString());
    }

    {
        // https://unicove.com/en/vaulta/contract/eosio/tables/rexbal/eosio?lower=123ibitu1234&upper=123ibitu1234
        const someRandomAccount = '123ibitu1234';

        const allRewards = await contract.readonly('getrewards', {
            account: someRandomAccount,
            mature_only: false
        });
        console.log('All Rewards:', allRewards.toString());

        const matureRewards = await contract.readonly('getrewards', {
            account: someRandomAccount,
            mature_only: true
        });
        console.log('Mature Rewards:', matureRewards.toString());
    }
})();