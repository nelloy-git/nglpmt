const testAddon = require('./build/Debug/testaddon.node')

const {
    Worker, isMainThread, parentPort, workerData
} = require('node:worker_threads');

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function demo(ctx) {
    for (let i = 0; i < 10; i++) {
        if (isMainThread){
            await ctx.start();
            await ctx.wait();
        }
    }
    console.log('Done JS');
}

let ctx
if (isMainThread) {
    const worker = new Worker(__filename);
    ctx = new testAddon.Context('Test');
    // ctx.onRun((ms) => {console.log("Main: " + ms + " ms");}, 30);
    ctx.onRun((ms) => {}, (ms) => {console.log("Main: " + ms + " ms"); return true;});
    // ctx.onKey((key_data) => {console.log(key_data.key + " " + key_data.action)});

    demo(ctx);
} else {
    ctx = new testAddon.Context('Test');
    ctx.onRun((ms) => {console.log("Worker: " + ms + " ms");}, 3);

    demo(ctx)
}
