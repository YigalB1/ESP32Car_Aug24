$fn=90;
//import("esp_car_pcb.stl");

//pcb_holes();
connect_holes(); // connect tocar

//screw_holder();




module pcb_screw_holders() {    
    h_shift=1;
    translate([4-0.5,-70+2,-h_shift])  screw_holder();
    translate([7.3,-5+2,-h_shift])     screw_holder();
    translate([63.2,-17+0.2,-h_shift]) screw_holder();
    translate([78,-75.9,0+-h_shift])   screw_holder();
} // of pcb_holes()

module connect_holes(_l=30,_w=10) {
    // holes to connect to the car and optional
    // these are the real holes to connect to the car....
    x1 = 110;
    y1 = 22;
    translate([-20+5,30,0]) color("red")  union() {
        translate([0,0,0])   cyl3();
        translate([x1,0,0])  cyl3();
        translate([x1,y1,0]) cyl3();
        translate([0,y1,0])  cyl3();
    } // of union()

    
    // Holes to allow better connection to top layer
    // and maybe other things
    
    for (i=[-20:20:_l-20]){
        translate([i,4,0])   cyl3();
        translate([i,_w-4,0])   cyl3();
    }    
    
        for (i=[9:15:_w-5]){
        translate([-20,i,0]) color("green")  cyl3();
        translate([_l-30,i,0]) color("green")  cyl3();
        
    }    

    
} // of connect_holes()




module cyl2() {
    color("red") cylinder(d=2.5,h=30,center=true);
} 

module cyl3() {
    color("red") cylinder(d=3.5,h=30,center=true);
} 

module screw_holder() {
    difference() {
        h1=15;
        h2=8;
        cylinder(h=h1,d1=9,d2=7);
        translate([0,0,h1-h2+1]) cylinder(h=h2,d=3.6);
    } // of difference()
} // of screw_holder