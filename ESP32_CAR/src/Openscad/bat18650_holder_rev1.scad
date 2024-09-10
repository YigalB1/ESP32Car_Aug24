pin_h = 10;
pin_d = 7;
hole_d = 2.8;
//pin_d = hole_d-0.4;
holes_dx = 91.7+hole_d/2;
holes_dy = 21.8+hole_d/2;

m2_insert_d1 = 2.8; // FYI
m2_insert_d2 = 3.4; // FYI
m2_d = 3.1;         // to be used
m2_h = 3.9+0.5;     // depth of hole for insert


// pins shifting
x_shift = holes_dx/2+1;
y_shift = holes_dy/2+1;
//h_shift = box_h/2-pin_h/2;
switch_x_dist = x_shift-10;
switch_y_dist = -25;
switch_h_dist = -5;

delta=3;
plate_x = 2 * x_shift + delta;
plate_y = 2 * y_shift + delta;
plate_h = 1;
   
bat18650_holder( _negative=0);
//bat18650_holder( _negative=1);
    
module bat18650_holder(_negative=0) {
    // if _negative is 0: create the holder
    // if _negative is 1: create the negative for holes
    //      fpr the switch & charging
    if (_negative==0) {
        translate([0,0,pin_h/2]) 18650_pins(_pin=1);
        cube([plate_x,plate_y,plate_h],center=true);        
    } // of if()
    else {
        x = 8;
        y = 10;
        d = 16 ;
        translate([switch_x_dist,-holes_dy/2-5,y/2+plate_h+0.5])
            cube([x,d,y],center=true);
       translate([-switch_x_dist,-holes_dy/2-5,y/2+plate_h+0.5])
            cube([x,d,y],center=true);
    } // of else()
} // of ()


module 18650_pins(_pin=1) {
    // _pin = 1:  holes for inserts
    // _pin = 0:  pins only
    
    translate([ x_shift, y_shift,0])
        holding_pin(_d=pin_d,_h=pin_h,_pin_only=_pin);
    translate([ x_shift,-y_shift,0])
        holding_pin(_d=pin_d,_h=pin_h,_pin_only=_pin);
    translate([-x_shift, y_shift,0])
        holding_pin(_d=pin_d,_h=pin_h,_pin_only=_pin);
    translate([-x_shift,-y_shift,0])
         holding_pin(_d=pin_d,_h=pin_h,_pin_only=_pin);
} // of 18650_pins()



module holding_pin(_d,_h,_pin_only=0) {
    // pins_only = 0 have holes for inserts
    // pins_only = 1 have malepins to hold the pcb
    if (_pin_only == 0) {
        // for inserts screws
        difference(){
            //color("red") cylinder(d=_d,h=_h,$fn=60,center=true);
            color("cyan") cube([_d,_d,_h],center=true);
            //cylinder(d=_d,h=_h,$fn=60,center=true);
            translate ([0,0,_h-m2_h])
            cylinder(d=m2_d,h=_h,$fn=60,center=true);
        } // of difference()
    } // of if()
    else {
        // just a pin
        cylinder(d=2.2,h=_h,$fn=60,center=true);
    } // of else 
} // of holding_pin()