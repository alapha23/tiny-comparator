#define NODE_TYPE 	int

#define tree_list	0x000

#define constructor	0x001


// result_decl: result of all the decl in the scope

#define var_decl	0x101
#define function_decl	0x102
#define identifier_node	0x103

#define function_type 	0x104
// the return type of the function
#define pointer_type	0x105
// the type of the obj pointed by the pointer
#define type_decl	0x106
#define integer_type	0x107
#define array_type	0x108
#define boolean_type	0x109
#define complex_type	0x10a
#define mult_expr	0x10b
#define trunc_div_expr	0x10c
#define trunc_mod_expr	0x10d
#define postincrement_expr	0x10e
#define postdecrement_expr	0x10f
#define preincrement_expr	0x110
#define predecrement_expr	0x111
#define call_expr	0x112
#define addr_expr	0x113
#define plus_expr	0x114
#define cond_expr	0x115
#define gt_expr		0x116
#define ge_expr		0x117
#define lt_expr		0x118
#define	le_expr		0x119
#define ne_expr		0x11a
#define eq_expr		0x11b
#define indirect_ref	0x11c
#define pointer_plus_expr	0x11d
#define nop_expr	0x11d
#define goto_expr	0x11e
#define label_expr	0x11f
#define convert_expr	0x120	// used in casting
#define return_expr     0x121
#define decl_expr       0x122
#define modify_expr     0x123
#define result_decl	0x124
#define bind_expr	0x125
#define switch_expr	0x126
#define case_label_expr	0x127
#define minus_expr	0x128


#define integer_cst	0x201
#define string_cst	0x202

#define statement_list	0x1000


// I defined this 
// 
#define scope_start	0x2000
