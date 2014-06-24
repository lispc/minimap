var gx = 103.811516;
var gy = 1.2744;
var map;
var gmark;
var marks = [];
$(document).ready(function(){
	function initialize() {
		var mapOptions = {
		  center: new google.maps.LatLng(gy, gx),
		  zoom: 12
		};
		map = new google.maps.Map($("#map-canvas")[0],mapOptions);
		google.maps.event.addListener(map, 'dblclick', function(event) {
			var y = event.latLng.lat();
			var x = event.latLng.lng();
			gx = x;
			gy = y;
			console.log(x);
			console.log(y);
			map.setCenter(event.latLng);
			var marker = new google.maps.Marker({
						position: event.latLng,
						map: map,
						icon: "/static/spotlight-poi-green.png"
					});
			
		});
	}
	google.maps.event.addDomListener(window, 'load', initialize);
	$("#query").keyup(function(){
		var text = $("#query").val();
		console.log(text);
		$.ajax({
			url:"/q",
			data:{x:gx,y:gy,s:text},
			dataType:"json",
			success:function(res){
				if(gmark)
					gmark.setMap(null);
				for(var j in marks){
					marks[j].setMap(null);
				}
				var html = '<table class="table" style="border:1"><thead><tr><th>Results</th></tr></thead><tbody>';
				console.log(res);
				for(var i in res.data){
					html+='<tr><td><a class="rmap" data-dx="'+res.data[i].x;
					html+='" data-dy="'+res.data[i].y+'">'+res.data[i].text;
					//html+=" ["+res.data[i].y+"E,"+res.data[i].x+"N]";
					html+="</a></td></tr>";
				}
				html += "</tbody></table>";
				$("#results").html(html);
				$(".rmap").each(function(){
					var dx = $(this).data("dx");
					var dy = $(this).data("dy");
					var marker = new google.maps.Marker({
						position: new google.maps.LatLng(dy,dx),
						map: map
					});
					marks.push(marker);
				})
			}
		})

	})
})