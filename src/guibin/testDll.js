const ffi = require('ffi-napi')
const ref = require('ref-napi')
const iconv = require('iconv-lite')
let buffer1 = Buffer.from("woo oom moon noox", 'utf8')
let buffer2 = Buffer.alloc(200)

const myDll = ffi.Library('./core.dll', {
    gen_chains_all: ['int',
        ['string', 'int', 'string']],
    gen_chain_word: ['int',
        ['string', 'int', 'string', 'char', 'char', 'char', 'bool']],
    gen_chain_char: ['int',
        ['string', 'int', 'string', 'char', 'char', 'char', 'bool']],
    get_execution_time: ['double',
        []],
    getResult: ['string',
        []]
})

let res;
res = myDll.gen_chain_char("woo oom moon noox" + '\x1a', 0, "", 0, 0, 0, false);
console.log(res, myDll.getResult())
res = myDll.gen_chain_char("woo oom moon noox" + '\x1a', 0, "", 0, 0, 0, false);
console.log(res, myDll.getResult())
res = myDll.gen_chain_word("woo oom moon noox" + '\x1a', 0, "", 0, 0, 0, false);
console.log(res, myDll.getResult())