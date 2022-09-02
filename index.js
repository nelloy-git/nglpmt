const testAddon = require('./build/Debug/testaddon.node')

async function emit(event){
    await event.emit(1, undefined, {a: "a"});
}

let event = new testAddon.Event();
event.push((a, b, c)=>{console.log(a, b, c)});
emit(event);

delete testAddon;