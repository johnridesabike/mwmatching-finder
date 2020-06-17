module Table: {
  [@react.component]
  let make:
    (
      ~vertices: array(Graph.Vertex.t),
      ~editVertex: Graph.Vertex.t => unit,
      ~editEdge: (Graph.Vertex.t, Graph.Vertex.t, option(float)) => unit,
      ~mates: Blossom.Match.t(Graph.Vertex.t, Graph.Vertex.Cmp.identity),
      ~graph: Graph.t,
      ~disabled: bool=?,
      ~style: ReactDOMRe.Style.t=?
    ) =>
    React.element;
};

[@react.component]
let make:
  (
    ~vertices: array(Graph.Vertex.t),
    ~graph: Graph.t,
    ~dispatch: (. Graph.action) => unit,
    ~cardinality: Blossom.Match.cardinality,
    ~setCardinality: (. (
                          Blossom.Match.cardinality => Blossom.Match.cardinality
                        )) =>
                     unit,
    ~mates: Blossom.Match.t(Graph.Vertex.t, Graph.Vertex.Cmp.identity),
    ~style: ReactDOMRe.Style.t=?
  ) =>
  React.element;
