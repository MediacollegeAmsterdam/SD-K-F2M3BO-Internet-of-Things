let testArray = ["on", 10, "off", 20, "on", 30 ];
let txtStart = '["on", 10, "off", 20, "on", 30 ]';
let arrayControl = JSON.parse(txtStart);
let nodeList = document.querySelectorAll("input"); // list of HTML <input> elements
let nodeList2 = document.querySelectorAll("value"); // list of HTML <value> elements
let debug = true;
console.log(nodeList2);
// uncomment for multi user
let refreshTimer = window.setInterval(ajaxReadJson, 10000); // timer get data from server
let arrayRemote;
function initNodes(){
    for (let i=0  ; i  < nodeList.length ; i++ ){
        nodeList[i].addEventListener('click', function(event ){setControls(event, nodeList[i], i)});
        nodeList[i].buttonStatus = ""; // add buttonStatus to HTML element
    }
}

function setControls(event, obj, controlNumber){
    if(debug) console.log(controlNumber + " " + obj.id + " " + obj.value + " " + obj.type);
    //is this a button?
    if(obj.type == "button"){
       if (obj.buttonStatus == "on") {
            obj.buttonStatus = "off";  // if button = on make it off viceversa 
            obj.style.backgroundColor = "red"; 
            arrayControl[controlNumber] = "off"; // change value in control array
       } 
       else {
            obj.buttonStatus = "on"; // if button = on make it off viceversa
            obj.style.backgroundColor = "green";
            arrayControl[controlNumber] = "on"; // change value in control array
       }
    }
    //is this a slider?
    if(obj.type == "range"){
        arrayControl[controlNumber] = obj.value; // change the range value in control array
    }
    // tell the world
    ajaxWriteJson(arrayControl);
}

function readValuesFromServer(arrayControl, obj, obj2){
        // readJSON from server and translate to HTML 
    for (let i=0  ; i  < obj.length ; i++ ){  
        if(obj[i].type == "button"){
            obj[i].buttonStatus = arrayControl[i];
            (obj[i].buttonStatus == "off") ? obj[i].style.backgroundColor = "red" : obj[i].style.backgroundColor = "green";                
           } 
        if(obj[i].type == "range"){
            obj[i].value = arrayControl[i];
        }
    }
     // readJSON from server and write values to HTML
     let j = 0; 
     for (let i=0  ; i  < obj.length ; i++ ){
       if(obj[i].type == "range"){
           obj2[j].innerHTML = arrayControl[i];
           j++;
        }
    }
}

function ajaxWriteJson(data){
    (debug) ? console.log(data) : ""; // debug
    let controlJsonString = JSON.stringify(data);// object to string
    let url = controlScript + "?src=html&data=" + controlJsonString;
    (debug) ? console.log(url) : ""; // debug
    makeAjaxCall(url, "GET"). then (ajaxReadJson);
  }

function ajaxReadJson(){
  makeAjaxCall(jsonFile,"POST"). then (readJson) ; // read control data
  makeAjaxCall(jsonFileRemote,"POST"). then(readJson2); // read sensor data
}
function readJson(jsonString) {
    arrayControl = JSON.parse(jsonString);//extract to JSON object
    readValuesFromServer(arrayControl, nodeList, nodeList2);
    }

function readJson2(jsonString) {
    arrayRemote = JSON.parse(jsonString);//extract to JSON object
    document.getElementById('temperature').innerHTML=" " + arrayRemote[1] +  "&deg;C";
    document.getElementById('humidity').innerHTML=" " + arrayRemote[3] +  "%";
    document.getElementById('heatindex').innerHTML=" " + arrayRemote[5] +  "&deg;C";
    }
    
    
function init(){
    initNodes(); //prepare HTML elements
    ajaxReadJson(); // get JSON string with values from server
}

init(); // start