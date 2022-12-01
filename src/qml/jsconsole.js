.pragma library

const scope = {
    thing : function() {
        console.log("Hi")
    }
}

function make_console_scope() {
    return new Function('return eval(arguments[0])')
}

function call(msg, current_func) {
    const exp = msg.toString()
    console.log(exp)
    const data = {
        expression : msg,
        result: "",
        error: ""
    }
    try {
        //const fun = new Function('return (' + exp + ')')
        let fun = current_func
        data.result = JSON.stringify(fun.call(scope, exp), null, 2)
        console.log('scope: ' + JSON.stringify(scope, null, 2), 'result: ' + data.result)
    } catch(e) {
        console.log(e.toString())
        data.error = e.toString()
    }
    return data
}

function get_obj_keys(o) {
    var keys = [];
    for(var key in o) {
        if(o.hasOwnProperty(key)) { //to be safe
            keys.push(key);
        }
    }
    return keys
}

function autocomplete(msg) {
    const replacements = {
        set_to : "",
        options : ""
    }

    // find a . somewhere
    let last_p = msg.lastIndexOf(".")

    let host = msg.substring(0,last_p)
    let partial_key = msg.substring(last_p)



    const last = msg.slice(-1)
    if (last === ".") {
        // whatever the last object is, all keys are options, sorted

        let target_as_str = msg.substring(0, msg.length - 1)

        const fun = new Function('return (' + target_as_str + ')')

        let target = fun.call(scope)

        replacements.options = get_obj_keys(target)

    } else {
        // this is a key of something



        if (!target.length){
            // no .
            // bail for now
        } else {
            // eval and get keys

            const fun = new Function('return (' + target_as_str + ')')

            let target = fun.call(scope)

            let keys = get_obj_keys(target)

            //filter based on what is similar
        }
    }

    return replacements
}
