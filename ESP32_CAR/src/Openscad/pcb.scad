$fn=90;
//import("esp_car_pcb.stl");

//pcb_holes();
car_holder(); // connect tocar


module pcb_holes() {
    rotate([0,0,0]) pins();
}



module pins() {
    
    translate([4-0.5,-70+2,0]) cyl2();
    translate([7.3,-5+2,0]) cyl2();
    translate([63.2,-17+0.2,0]) cyl2();
    translate([78,-75.9,0]) cyl2();
    
} // of pcb_holes()



module car_holder() {
    dx = 85.2;
    dy = 80.2;
    color("cyan") translate([0,0,0]) cyl3();
    color("cyan") translate([dx,0,0]) cyl3();
    color("cyan") translate([dx,dy,0]) cyl3();
    color("cyan") translate([0,dy,0]) cyl3();

} // of car_holder()




module cyl2() {
    color("red") cylinder(d=2,h=30,center=true);
} 

module cyl3() {
    color("red") cylinder(d=3,h=30,center=true);
} 