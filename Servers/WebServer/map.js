/*发送一个ajax请求，将所有垃圾桶的信息都获取到*/
window.onload = function () {
    /*服务端所有垃圾桶信息文件的json对象*/
    var allTrashesInfo;
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4) {
            allTrashesInfo = JSON.parse(xhr.responseText);
            //console.log(allTrashesInfo);
            drawMap(allTrashesInfo);
        }
    }
    /*ajax发送一个get请求*/
    xhr.open("get", "getAllTrashesInfo");
    xhr.send();
}

function drawMap(allTrashesInfo) {
    /*
        console.log("typeof allTrasherInfo:" + typeof (allTrashesInfo));
        console.log("length of allTrashesInfo:" + allTrashesInfo.length);
        console.log("typeof allTrashesInfo[0]:" + typeof (allTrashesInfo[0]));
    */
    var map = new BMapGL.Map('container');
    map.enableScrollWheelZoom(true);
    map.setMapStyleV2({ styleId: "849c929468cab3ac147a2d3f5f364347" });

    /*设置地图中心点，是hdu*/
    var centralPoint = new BMapGL.Point(120.3498, 30.3199);
    map.centerAndZoom(centralPoint, 17);

    var n = allTrashesInfo.length;
    var pointsArray = new Array(n);
    var markersArray = new Array(n);
    var infoWindowArray = new Array(n);
    var myIconArray = new Array(n);

    var opts = {
        width: 200,
        height: 50,
        title: "Usage:"
    }

    for (var i = 0; i < n; i++) {
        pointsArray[i] = new BMapGL.Point(allTrashesInfo[i]["location"][0], allTrashesInfo[i]["location"][1]);
        myIconArray[i] = new BMapGL.Icon(allTrashesInfo[i]["recyleBitMap"] + ".png", new BMapGL.Size(80, 80));

        markersArray[i] = new BMapGL.Marker(pointsArray[i], { icon: myIconArray[i] });

        infoWindowArray[i] = new BMapGL.InfoWindow("Trash-" + allTrashesInfo[i]["id"] + " usage:" + allTrashesInfo[i]["usage"], opts);

        markersArray[i].info = infoWindowArray[i];

        markersArray[i].addEventListener('click', function () {
            /*
                有必要吐槽一下，上面之所以要把infoWindow设置成marker的一个属性，
                就是为了这个click事件的回调函数，由于回调函数的执行是异步的，不能用i简单的区分，
                否则的话每次执行都会用退出循环时的i值。
                所以将其绑定到this上，就能区分不同的marker的点击事件了。
            */
            map.openInfoWindow(this.info, this.getPosition());
        });
        map.addOverlay(markersArray[i]);
    }

}
