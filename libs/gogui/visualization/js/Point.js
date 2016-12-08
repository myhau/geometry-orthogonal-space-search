function Point( xParam, yParam, stateParam ) {
	
	this.x = xParam;
	this.y = yParam;
	this.state = stateParam;
}

Point.prototype = {
	
	constructor: Point, 
	
	getX : function() {
		return this.x;
	},
	getY : function() {
		return this.y;
	},
	getState : function() {
		return this.state;
	}
}
