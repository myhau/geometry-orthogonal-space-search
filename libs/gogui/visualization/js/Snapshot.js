function Snapshot() {
	
	this.visibleLines = [];
	this.visiblePoints = [];

}

Snapshot.prototype = {

	constructor : Snapshot,
	
	setLines : function( linesParam ) {
		this.visibleLines = linesParam;
	},
	
	setPoints : function( pointsParam ) {
		this.visiblePoints = pointsParam;
	},
	
	getPoints : function() {
		return this.visiblePoints;
	},
	
	getLines : function() {
		return this.visibleLines;
	}
}
