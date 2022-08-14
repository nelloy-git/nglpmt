const testAddon = require('./build/Debug/testaddon.node')

ctx = new testAddon.Context({
    title: "Title",
    width: 1024,
    height: 480
});
ctx.onRun((us) => {console.log(us + " us");});
// ctx.onKey((key_data) => {console.log(key_data.key + " " + key_data.action)});

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function demo() {
    for (let i = 0; i < 30; i++) {
        console.log(`Waiting ${i/10} seconds...`);
        ctx.start();
        await sleep(100);
    }
    console.log('Done');
}

demo();