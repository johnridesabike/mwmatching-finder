module Vertex: {
  type t;
  type vertex = t;

  let make: string => option(t);

  let toString: t => string;

  let toElement: t => React.element;

  let empty: t;

  let eq: (t, t) => bool;

  module Cmp: {type identity;};

  module Set: {
    type t;
    let has: (t, vertex) => bool;
  };
};

module Edge: {type t;};

type t;

let empty: t;

let encode: t => Js.Json.t;

let decode: Js.Json.t => t;

let edgeCount: t => int;

let vertices: t => Vertex.Set.t;

type action =
  | AddVertex(Vertex.t)
  | SetEdge(Vertex.t, Vertex.t, float)
  | RemoveEdge(Vertex.t, Vertex.t)
  | RemoveVertex(Vertex.t)
  | RenameVertex({
      old: Vertex.t,
      new_: Vertex.t,
    })
  | Set(t);

let reducer: (t, action) => t;

let toList: t => list((Vertex.t, Vertex.t, float));

let fromList: list((Vertex.t, Vertex.t, float)) => t;

let verticesToArray: t => array(Vertex.t);

let getEdge: (t, Vertex.t, Vertex.t) => option(float);

module Mates: {
  type graph = t;

  type t = Blossom.Match.t(Vertex.t, Vertex.Cmp.identity);

  let empty: t;

  let useMates: (~cardinality:Blossom.Match.cardinality=?, graph) => t;
};
