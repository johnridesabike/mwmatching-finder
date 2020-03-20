module Edge: {
  type t = (string, string);
  module Cmp: {type identity;};
};

type t = {
  vertices: Belt.Set.String.t,
  edges: Belt.Map.t(Edge.t, float, Edge.Cmp.identity),
};

let empty: t;

let encode: t => Js.Json.t;

let decode: Js.Json.t => t;

type action =
  | AddVertex(string)
  | SetEdge(string, string, float)
  | RemoveEdge(string, string)
  | RemoveVertex(string)
  | RenameVertex({
      old: string,
      new_: string,
    })
  | Set(t);

let reducer: (t, action) => t;

let toList: t => list((string, string, float));

let fromList: list((string, string, float)) => t;

let verticesToArray: t => array(string);

let getEdge: (t, string, string) => option(float);

let useMates:
  (t, Blossom.Match.cardinality) =>
  Blossom.Match.t(string, Blossom.Match.String.Cmp.identity);
