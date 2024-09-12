$fn=90;

use <pcb.scad>
use <bat18650_holder_rev1.scad>
use <microservo-sg90-9g.scad>

base_l = 130;
base_w = 90-10; // the +10 is to allow more space to the servo's on side 
base_h = 2;
walls_h = 10; // was 30

// w = 19.2 l = 120

base_bat_plate() ;
//base_pcb_plate() ;


module base_pcb_plate() {
    shift = 18;
    difference() {
        union() {
            color("red") cube([base_l,base_w,base_h],center=true);  
            translate([-50+10,30+8,0]) pcb_holes();    
            translate([-40,-40,0]) connect_holes(base_l,base_w);
        } // of union()
        
        for (j = [-base_l/2+5:30:base_l/2-30]){
            for (i = [-20,0,20]){
            translate([j+20,i,-5]) cylinder(d=10,h=10,$fn=60);
            } // of inner FOR
        } // of outer FOR
        
        
        //translate([-50+10,30+8,0]) pcb_holes();
        
        translate([-40,-40,0]) connect_holes(base_l,base_w);
        
    } // of difference()
} // of base_pcb_plate()



module base_bat_plate() {
    shift = 18;
    difference() {
        color("red") cube([base_l,base_w,base_h],center=true);
        translate([0,-shift,0]) bat_holes(); // 1st battery
        translate([0,+shift,0]) bat_holes(); // 2nd battery
        floor_holes();
        
        // top servos are on the PCB level
        //translate([base_l/2-10,0,-10]) rotate([0,0,0]) servo(); // Servo
        //translate([-base_l/2+10,0,-10]) rotate([0,0,0]) servo(); // Servo
        dx=37.5; // the point were they merge from both sides
        dy=36+5;
        // side servos - no servos on print. mounetd on car.
        //translate([base_l/4-dx,dy,-10]) rotate([0,0,90]) servo(); // Servo
        //translate([-base_l/4+dx,-dy,-10]) rotate([0,0,90]) servo(); // Servo
        
        translate([-40,-40,0]) connect_holes(base_l,base_w);
        
    } // of difference()
} // of base_bat_plate()



module floor_holes() {
    
   for (j = [-base_l/2+30-0:22:base_l/2-30-5]){
    for (i = [-20,0,20]){
        translate([j+10,i,-5]) cylinder(d=10,h=10,$fn=60);
    } // of inner FOR
        } // of outer FOR
} // of floor_holes()

module bat_holes() {
    hole_d = 3;
    holes_dx = 91.7+hole_d/2;
    holes_dy = 21.8+hole_d/2;
    x_shift = holes_dx/2+1;
    y_shift = holes_dy/2+1;
    pin_h = 10;
    pin_d = 3;
    
    translate([ x_shift, y_shift,0])
        cylinder(d=pin_d,h=pin_h,center=true);
    translate([ x_shift,-y_shift,0])
        cylinder(d=pin_d,h=pin_h,center=true);
    translate([-x_shift, y_shift,0])
        cylinder(d=pin_d,h=pin_h,center=true);
    translate([-x_shift,-y_shift,0])
        cylinder(d=pin_d,h=pin_h,center=true);
} //of bat_holes()





