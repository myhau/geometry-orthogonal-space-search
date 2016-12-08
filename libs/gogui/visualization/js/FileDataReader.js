
function FileDataReader( filenameParam, callback, callbackContext ) {
	
	var filename = filenameParam;
	
	this.snapshots = [];
	this.lines = [];
	this.points = [];
	
	this.xMin = null;
	this.yMin = null;
	this.xMax = null;
	this.yMax = null;
	
	function populateArrays( _that, linesArray, pointsArray, snapshotsArray ) {
	
		$.getJSON( filename, function( json ) {
			
			_that.xMin = json.points[0].x;
			_that.yMin = json.points[0].y;
			_that.xMax = json.points[0].x;
			_that.yMax = json.points[0].y;
			
			for( var i = 0; i < json.history.length; ++i ) {
				var s = new Snapshot();
				s.setPoints( json.history[i].points );
				s.setLines( json.history[i].lines );
				snapshotsArray.push(s);
			}
			
			for( var i = 0; i < json.points.length; ++i ) {
				
				pointsArray.push( json.points[i] );
				
				if( pointsArray[i].x > _that.xMax ) {
					_that.xMax = pointsArray[i].x;
				}
				if( pointsArray[i].x < _that.xMin ) {
					_that.xMin = pointsArray[i].x;
				}
				if( pointsArray[i].y > _that.yMax ) {
					_that.yMax = pointsArray[i].y;
				}
				if( pointsArray[i].y < _that.yMin ) {
					_that.yMin = pointsArray[i].y;
				}
			}
			
			for( var i = 0; i < json.lines.length; ++i ) {
				linesArray.push(json.lines[i]);
			}
			
			callback(callbackContext);
			
		});
		
	}
	
	populateArrays( this, this.lines, this.points, this.snapshots );
		
		
}

FileDataReader.prototype = {
	
	constructor : FileDataReader,
	
	getLines : function() {
		return this.lines;
	},
	
	getPoints : function() {
		return this.points;
	},
	
	getSnapshots : function() {
		return this.snapshots;
	},
	
	getXMin: function() {
		return this.xMin;
	},
	
	getXMax: function() {
		return this.xMax;
	},
	
	getYMin: function() {
		return this.yMin;
	},
	
	getYMax: function() {
		return this.yMax;
	}
	
}
