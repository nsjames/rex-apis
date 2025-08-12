# Rex APIs Contract

This is a small contract that provides read-only actions (view functions) for 
Vaulta staking.

## Usage

### Using JavaScript

You can take a look at the `javascript/example.ts` file for a complete example of how to use this contract in JavaScript.

---

First, create a `client` and `contract` instance:

```javascript
import { APIClient, Chains } from "@wharfkit/session"
import { ContractKit } from "@wharfkit/contract"

const client = new APIClient({ url: Chains.Vaulta.url });
const kit = new ContractKit({ client });
const contract = await kit.load('userexapisv1');
```

#### `convert`: Converts between REX and Vaulta

```javascript
const result = await contract.readonly('convert', {
    rex_or_vaulta: '1.0000 A', // Will return REX amount
    // or
    rex_or_vaulta: '1.0000 REX', // Will return A amount
});
```

#### `getapy`: Returns the current annual percentage yield (APY) for REX

```javascript
const result = await contract.readonly('getapy');
// Example: 10.12
```

- `getrewards`: Returns the rewards for a given account

The `mature_only` boolean parameter indicates whether to return only mature rewards (REX that has been fully unstaked) or all rewards.

```javascript
const result = await contract.readonly('getrewards', {
    account: 'some.account',
    mature_only: false
});
```

### Using Unicove's API Wrapper

https://unicove.com/vaulta/contract/userexapisv1/actions/getrewards/008064002b691e9e01?readonly=false&triggerOnPageLoad=false

## Using this contract

## Building

### Using Bolt

If you have JavaScript installed on your system, you can build the contract using the following command:

```shell
npx @vaulta/bolt build
```

### Using CDT

If you have a local CDT installation, you can build the contract using the following command:

```shell
cdt-cpp -o build/contract.wasm contracts/contract.cpp --contract=rexapis --abigen
```