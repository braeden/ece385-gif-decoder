// module frame_state_machine ( 
//     input logic Clk, Reset, DrawStart
//     // input logic [9:0] DrawX, DrawY,

//     output logic BEGIN, SETUP

// );

//     enum logic [4:0] {NOOP, DRAW} State, Next_state;


    
// 	always_ff @ (posedge Clk)
// 	begin
// 		if (Reset) 
// 			State <= NOOP;
// 		else 
// 			State <= Next_state;
// 	end
//     always_comb
//     begin
//         Next_state = State;

//         BEGIN = 1'b0;
//         SETUP = 1'b0;
//         unique case (State)
//             NOOP:
//                 Next_state = DrawStart ? DRAW : NOOP;
//             DRAW: begin
//                 BEGIN = 1'b1;
//                 Next_state = DRAW;
//             end
//             default: 
//                 Next_state = NOOP;
//         endcase
//     end

// endmodule