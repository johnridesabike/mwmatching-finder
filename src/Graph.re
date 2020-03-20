open Belt;
module M = Map;
module S = Set;

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

let useMates = (graph, cardinality) => {
  let (matches, setMatches) =
    React.useState(() => graph->toList->Blossom.Match.String.make);
  React.useEffect2(
    () => {
      setMatches(state =>
        switch (graph->toList->(Blossom.Match.String.make(~cardinality))) {
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
