// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_Id = require("bs-platform/lib/js/belt_Id.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Float = require("bs-platform/lib/js/belt_Float.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Match$Blossom = require("re-blossom/src/Match.bs.js");
var Belt_SetString = require("bs-platform/lib/js/belt_SetString.js");
var Caml_primitive = require("bs-platform/lib/js/caml_primitive.js");
var Formality__Form = require("re-formality/src/Formality__Form.bs.js");
var Caml_js_exceptions = require("bs-platform/lib/js/caml_js_exceptions.js");
var Formality__PublicHelpers = require("re-formality/src/Formality__PublicHelpers.bs.js");

function cmp(param, param$1) {
  var z = param$1[1];
  var y = param$1[0];
  var b = param[1];
  var a = param[0];
  var match = Caml_primitive.caml_string_compare(a, y);
  var match$1 = Caml_primitive.caml_string_compare(b, z);
  var exit = 0;
  if (match !== 0 || match$1 !== 0) {
    exit = 1;
  } else {
    return 0;
  }
  if (exit === 1) {
    var match$2 = Caml_primitive.caml_string_compare(a, z);
    var match$3 = Caml_primitive.caml_string_compare(b, y);
    if (match$2 === 0 && match$3 === 0) {
      return 0;
    }
    var e = match$2 + match$3 | 0;
    if (e !== 0) {
      return e;
    } else {
      return match$2;
    }
  }
  
}

var Cmp = Belt_Id.MakeComparable({
      cmp: cmp
    });

var Edge = {
  Cmp: Cmp
};

var empty_edges = Belt_Map.make(Cmp);

var empty = {
  vertices: Belt_SetString.empty,
  edges: empty_edges
};

function reducer(graph, action) {
  var edges = graph.edges;
  var vertices = graph.vertices;
  switch (action.tag | 0) {
    case /* AddVertex */0 :
        return {
                vertices: Belt_SetString.add(vertices, action[0]),
                edges: graph.edges
              };
    case /* AddEdge */1 :
        var j = action[1];
        var i = action[0];
        return {
                vertices: Belt_SetString.add(Belt_SetString.add(vertices, i), j),
                edges: Belt_Map.set(edges, /* tuple */[
                      i,
                      j
                    ], action[2])
              };
    case /* RemoveVertex */2 :
        var vertex = action[0];
        return {
                vertices: Belt_SetString.remove(vertices, vertex),
                edges: Belt_Map.keep(edges, (function (param, _weight) {
                        if (param[0] !== vertex) {
                          return param[1] !== vertex;
                        } else {
                          return false;
                        }
                      }))
              };
    case /* RemoveEdge */3 :
        return {
                vertices: graph.vertices,
                edges: Belt_Map.remove(edges, /* tuple */[
                      action[0],
                      action[1]
                    ])
              };
    
  }
}

function fromList(l) {
  var _acc = empty;
  var _l = l;
  while(true) {
    var l$1 = _l;
    var acc = _acc;
    if (l$1) {
      var match = l$1[0];
      _l = l$1[1];
      _acc = reducer(acc, /* AddEdge */Block.__(1, [
              match[0],
              match[1],
              match[2]
            ]));
      continue ;
    } else {
      return acc;
    }
  };
}

function toList(param) {
  return Belt_Map.reduce(param.edges, /* [] */0, (function (acc, param, w) {
                return /* :: */[
                        /* tuple */[
                          param[0],
                          param[1],
                          w
                        ],
                        acc
                      ];
              }));
}

function verticesToList(param) {
  return Belt_SetString.toList(param.vertices);
}

var Graph = {
  S: /* alias */0,
  M: /* alias */0,
  empty: empty,
  reducer: reducer,
  fromList: fromList,
  toList: toList,
  verticesToList: verticesToList
};

var sampleGraph = fromList(/* :: */[
      /* tuple */[
        "Mary",
        "Joseph",
        40
      ],
      /* :: */[
        /* tuple */[
          "Mary",
          "Michael",
          40
        ],
        /* :: */[
          /* tuple */[
            "Joseph",
            "Michael",
            60
          ],
          /* :: */[
            /* tuple */[
              "Joseph",
              "Gabriel",
              55
            ],
            /* :: */[
              /* tuple */[
                "Michael",
                "Raphael",
                55
              ],
              /* :: */[
                /* tuple */[
                  "Gabriel",
                  "Raphael",
                  50
                ],
                /* :: */[
                  /* tuple */[
                    "Mary",
                    "Paul",
                    15
                  ],
                  /* :: */[
                    /* tuple */[
                      "Raphael",
                      "Peter",
                      30
                    ],
                    /* :: */[
                      /* tuple */[
                        "Peter",
                        "John",
                        10
                      ],
                      /* :: */[
                        /* tuple */[
                          "Paul",
                          "James",
                          10
                        ],
                        /* :: */[
                          /* tuple */[
                            "Gabriel",
                            "Andrew",
                            30
                          ],
                          /* [] */0
                        ]
                      ]
                    ]
                  ]
                ]
              ]
            ]
          ]
        ]
      ]
    ]);

function update(_state, value) {
  return {
          name: value
        };
}

function validator_validate(param) {
  if (param.name === "") {
    return /* Error */Block.__(1, ["Error"]);
  } else {
    return /* Ok */Block.__(0, [/* Valid */0]);
  }
}

var validator = {
  field: /* Name */0,
  strategy: /* OnFirstBlur */0,
  dependents: undefined,
  validate: validator_validate
};

var NameField = {
  update: update,
  validator: validator
};

var validators = /* :: */[
  validator,
  /* [] */0
];

var NameForm = {
  NameField: NameField,
  validators: validators
};

var NameFormHook = Formality__Form.Make({
      validators: validators
    });

function update$1(state, value) {
  return {
          i: value,
          j: state.j,
          w: state.w
        };
}

function validator_validate$1(param) {
  if (param.i === "") {
    return /* Error */Block.__(1, ["Pick a name"]);
  } else {
    return /* Ok */Block.__(0, [/* Valid */0]);
  }
}

var validator$1 = {
  field: /* I */0,
  strategy: /* OnFirstChange */1,
  dependents: undefined,
  validate: validator_validate$1
};

var IField = {
  update: update$1,
  validator: validator$1
};

function update$2(state, value) {
  return {
          i: state.i,
          j: value,
          w: state.w
        };
}

function validator_validate$2(param) {
  var j = param.j;
  if (j === "") {
    return /* Error */Block.__(1, ["Pick a name"]);
  } else if (param.i === j) {
    return /* Error */Block.__(1, ["Names must be different"]);
  } else {
    return /* Ok */Block.__(0, [/* Valid */0]);
  }
}

var validator$2 = {
  field: /* J */1,
  strategy: /* OnFirstChange */1,
  dependents: undefined,
  validate: validator_validate$2
};

var JField = {
  update: update$2,
  validator: validator$2
};

function update$3(state, value) {
  return {
          i: state.i,
          j: state.j,
          w: value
        };
}

function validator_validate$3(param) {
  if (param.w !== undefined) {
    return /* Ok */Block.__(0, [/* Valid */0]);
  } else {
    return /* Error */Block.__(1, ["Weight must be a valid number"]);
  }
}

var validator$3 = {
  field: /* W */2,
  strategy: /* OnFirstChange */1,
  dependents: undefined,
  validate: validator_validate$3
};

var WField = {
  update: update$3,
  validator: validator$3
};

var validators_001 = /* :: */[
  validator$2,
  /* :: */[
    validator$3,
    /* [] */0
  ]
];

var validators$1 = /* :: */[
  validator$1,
  validators_001
];

var MatchForm = {
  IField: IField,
  JField: JField,
  WField: WField,
  validators: validators$1
};

var MatchFormHook = Formality__Form.Make({
      validators: validators$1
    });

function Matching$MatchAdder(Props) {
  var graph = Props.graph;
  var dispatch = Props.dispatch;
  var form = Curry._2(MatchFormHook.useForm, {
        i: "",
        j: "",
        w: 0.0
      }, (function (param, form) {
          var w = param.w;
          if (w !== undefined) {
            Curry._1(dispatch, /* AddEdge */Block.__(1, [
                    param.i,
                    param.j,
                    w
                  ]));
          }
          Curry._1(form.notifyOnSuccess, undefined);
          return Curry._1(form.reset, /* () */0);
        }));
  var nameList = Belt_List.toArray(Belt_List.map(verticesToList(graph), (function (p) {
              return React.createElement("option", {
                          key: p,
                          value: p
                        }, p);
            })));
  var partial_arg = form.submit;
  var match = Curry._1(form.result, /* I */0);
  var tmp;
  tmp = match !== undefined && !match.tag ? false : true;
  var match$1 = Curry._1(form.result, /* I */0);
  var match$2 = Curry._1(form.result, /* J */1);
  var match$3 = Curry._1(form.result, /* W */2);
  var tmp$1;
  var exit = 0;
  var message;
  var exit$1 = 0;
  var exit$2 = 0;
  if (match$1 !== undefined) {
    var match$4 = match$1;
    if (match$4.tag) {
      message = match$4[0];
      exit = 1;
    } else {
      exit$2 = 3;
    }
  } else if (match$2 !== undefined) {
    exit$2 = 3;
  } else if (match$3 !== undefined) {
    exit$1 = 2;
  } else {
    tmp$1 = null;
  }
  if (exit$2 === 3) {
    if (match$2 !== undefined) {
      var match$5 = match$2;
      if (match$5.tag) {
        message = match$5[0];
        exit = 1;
      } else {
        exit$1 = 2;
      }
    } else {
      exit$1 = 2;
    }
  }
  if (exit$1 === 2) {
    if (match$3 !== undefined) {
      var match$6 = match$3;
      if (match$6.tag) {
        message = match$6[0];
        exit = 1;
      } else {
        tmp$1 = null;
      }
    } else {
      tmp$1 = null;
    }
  }
  if (exit === 1) {
    tmp$1 = React.createElement("p", undefined, message);
  }
  return React.createElement("form", {
              onSubmit: (function (param) {
                  return Formality__PublicHelpers.Dom.preventDefault(partial_arg, param);
                })
            }, React.createElement("select", {
                  value: form.state.i,
                  onChange: (function ($$event) {
                      return Curry._2(form.change, /* I */0, update$1(form.state, $$event.target.value));
                    })
                }, React.createElement("option", {
                      value: ""
                    }, "--select--"), nameList), " + ", React.createElement("select", {
                  disabled: tmp,
                  value: form.state.j,
                  onChange: (function ($$event) {
                      return Curry._2(form.change, /* J */1, update$2(form.state, $$event.target.value));
                    })
                }, React.createElement("option", {
                      value: ""
                    }, "--select--"), nameList), React.createElement("input", {
                  type: "number",
                  value: String(Belt_Option.getWithDefault(form.state.w, 0)),
                  onChange: (function ($$event) {
                      return Curry._2(form.change, /* W */2, update$3(form.state, Belt_Float.fromString($$event.target.value)));
                    })
                }), React.createElement("button", undefined, "Submit"), tmp$1);
}

var MatchAdder = {
  make: Matching$MatchAdder
};

function Matching(Props) {
  var match = React.useReducer(reducer, sampleGraph);
  var dispatch = match[1];
  var graph = match[0];
  var match$1 = React.useState((function () {
          return Match$Blossom.$$String.make(undefined, toList(graph));
        }));
  var setMatches = match$1[1];
  React.useEffect((function () {
          Curry._1(setMatches, (function (state) {
                  try {
                    return Match$Blossom.$$String.make(undefined, toList(graph));
                  }
                  catch (raw_e){
                    var e = Caml_js_exceptions.internalToOCamlException(raw_e);
                    console.error(e);
                    console.log(Belt_List.toArray(toList(graph)));
                    return state;
                  }
                }));
          return ;
        }), [graph]);
  var form = Curry._2(NameFormHook.useForm, {
        name: ""
      }, (function (param, form) {
          Curry._1(dispatch, /* AddVertex */Block.__(0, [param.name]));
          Curry._1(form.notifyOnSuccess, undefined);
          return Curry._1(form.reset, /* () */0);
        }));
  var partial_arg = form.submit;
  var match$2 = Curry._1(form.result, /* Name */0);
  var tmp;
  if (match$2 !== undefined) {
    var match$3 = match$2;
    tmp = match$3.tag ? match$3[0] : null;
  } else {
    tmp = null;
  }
  return React.createElement("div", undefined, React.createElement("form", {
                  onSubmit: (function (param) {
                      return Formality__PublicHelpers.Dom.preventDefault(partial_arg, param);
                    })
                }, React.createElement("p", undefined, "Add a name"), React.createElement("input", {
                      disabled: form.submitting,
                      value: form.state.name,
                      onBlur: (function (param) {
                          return Curry._1(form.blur, /* Name */0);
                        }),
                      onChange: (function ($$event) {
                          return Curry._2(form.change, /* Name */0, (form.state, {
                                        name: $$event.target.value
                                      }));
                        })
                    }), React.createElement("button", {
                      disabled: form.submitting
                    }, form.submitting ? "Submitting..." : "Submit")), tmp, React.createElement("h2", undefined, "People"), React.createElement("ul", undefined, Belt_List.toArray(Belt_List.map(verticesToList(graph), (function (p) {
                            return React.createElement("li", {
                                        key: p
                                      }, p);
                          })))), React.createElement("h2", undefined, "All potential matches"), React.createElement("table", undefined, React.createElement("tbody", undefined, Belt_List.toArray(Belt_List.mapWithIndex(toList(graph), (function (key, param) {
                                return React.createElement("tr", {
                                            key: String(key)
                                          }, React.createElement("td", undefined, param[0]), React.createElement("td", undefined, param[1]), React.createElement("td", undefined, String(param[2])));
                              }))))), React.createElement("h2", undefined, "Add potential match"), React.createElement(Matching$MatchAdder, {
                  graph: graph,
                  dispatch: dispatch
                }), React.createElement("h2", undefined, "Matches"), React.createElement("table", undefined, React.createElement("tbody", undefined, Belt_List.toArray(Belt_List.mapWithIndex(Match$Blossom.toList(match$1[0]), (function (key, param) {
                                return React.createElement("tr", {
                                            key: String(key)
                                          }, React.createElement("td", undefined, param[0]), React.createElement("td", undefined, param[1]));
                              }))))));
}

var make = Matching;

exports.Edge = Edge;
exports.Graph = Graph;
exports.sampleGraph = sampleGraph;
exports.NameForm = NameForm;
exports.NameFormHook = NameFormHook;
exports.MatchForm = MatchForm;
exports.MatchFormHook = MatchFormHook;
exports.MatchAdder = MatchAdder;
exports.make = make;
/* Cmp Not a pure module */
