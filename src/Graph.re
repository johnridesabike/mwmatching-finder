open Belt;
module M = Map;
module S = Set;

module Vertex = {
  type t = string;
  type vertex = t;

  let make =
    fun
    | "" => None
    | s => Some(s);

  external toString: t => string = "%identity";

  external toElement: t => React.element = "%identity";

  let empty = "";

  let cmp: (t, t) => int = compare;

  let eq: (t, t) => bool = (==);

  module Cmp =
    Id.MakeComparable({
      type nonrec t = t;
      let cmp = cmp;
    });

  module Set = Set.String;
};

module Edge = {
  type t = (string, string);
  module Cmp =
    Id.MakeComparable({
      type nonrec t = t;
      let cmp: (t, t) => int =
        ((a, b), (y, z)) =>
          switch (compare(a, y), compare(b, z)) {
          | (0, 0) => 0
          | (c, d) =>
            switch (c + d) {
            | 0 => c
            | e => e
            }
          };
    });
  let make = (i, j) =>
    switch (String.compare(i, j)) {
    | 1 => (i, j)
    | _ => (j, i)
    };

  let encode: t => Js.Json.t = Json.Encode.(tuple2(string, string));

  let decode: Js.Json.t => t = Json.Decode.(tuple2(string, string));
};

[@bs.deriving accessors]
type t = {
  /* A map of vertices to their neighbors */
  vertices: S.String.t,
  edges: M.t(Edge.t, float, Edge.Cmp.identity),
};

let encode = ({edges, vertices}) =>
  Json.Encode.(
    object_([
      ("vertices", vertices |> S.String.toArray |> array(string)),
      (
        "edges",
        edges |> M.toArray |> array(tuple2(Edge.encode, Json.Encode.float)),
      ),
    ])
  );

let decode = json =>
  Json.Decode.{
    vertices: json |> field("vertices", array(string)) |> S.String.fromArray,
    edges:
      json
      |> field("edges") @@
      array(tuple2(Edge.decode, Json.Decode.float))
      |> M.fromArray(~id=(module Edge.Cmp)),
  };

let emptyEdges = M.make(~id=(module Edge.Cmp));

let empty = {vertices: S.String.empty, edges: emptyEdges};

let edgeCount = ({edges, _}) => M.size(edges);

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

let reducer = ({vertices, edges} as graph, action) =>
  switch (action) {
  | Set(graph) => graph
  | AddVertex(vertex) => {
      ...graph,
      vertices: S.String.add(vertices, vertex),
    }
  | SetEdge(i, j, _) when i == j => graph
  | SetEdge(i, j, weight) => {
      vertices: vertices->S.String.add(i)->S.String.add(j),
      edges: M.set(edges, Edge.make(i, j), weight),
    }
  | RemoveEdge(i, j) => {vertices, edges: M.remove(edges, Edge.make(i, j))}
  | RemoveVertex(vertex) => {
      vertices: S.String.remove(vertices, vertex),
      edges: M.keep(edges, ((i, j), _w) => i != vertex && j != vertex),
    }
  | RenameVertex({old, new_}) => {
      vertices: vertices->S.String.remove(old)->S.String.add(new_),
      edges:
        M.reduce(edges, emptyEdges, (newEdges, (i, j), w) =>
          if (i == old) {
            M.set(newEdges, (new_, j), w);
          } else if (j == old) {
            M.set(newEdges, (i, new_), w);
          } else {
            M.set(newEdges, (i, j), w);
          }
        ),
    }
  };

let fromList = l => {
  let rec loop = (acc, l) =>
    switch (l) {
    | [] => acc
    | [(i, j, w), ...l] => loop(reducer(acc, SetEdge(i, j, w)), l)
    };
  loop(empty, l);
};

let toList = ({edges, _}) =>
  M.reduce(edges, [], (acc, (i, j), w) => [(i, j, w), ...acc]);

let verticesToArray = ({vertices, _}) => S.String.toArray(vertices);

let getEdge = ({edges, _}, i, j) => M.get(edges, Edge.make(i, j));

module Mates = {
  type graph = t;
  type t = Blossom.Match.t(Vertex.t, Vertex.Cmp.identity);

  let blossom = Blossom.Match.make(~id=(module Vertex.Cmp), ~cmp=Vertex.cmp);

  let empty = blossom([]);

  let useMates = (~cardinality=`NotMax, graph) => {
    let (matches, setMatches) = React.useState(() => graph->toList->blossom);
    React.useEffect2(
      () => {
        setMatches(state =>
          switch (graph->toList->(blossom(~cardinality))) {
          | exception e =>
            Js.Console.error(e);
            state;
          | matches => matches
          }
        );
        None;
      },
      (graph, cardinality),
    );
    matches;
  };
};
