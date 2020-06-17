module VertexEditor: {
  [@react.component]
  let make:
    (
      ~dispatch: (. Graph.action) => unit,
      ~onSubmit: unit => unit,
      ~name: Graph.Vertex.t,
      ~names: Graph.Vertex.Set.t
    ) =>
    React.element;
};

module VertexAdder: {
  [@react.component]
  let make:
    (
      ~dispatch: (. Graph.action) => unit,
      ~onSubmit: unit => unit,
      ~names: Graph.Vertex.Set.t
    ) =>
    React.element;
};

module EdgeSetter: {
  [@react.component]
  let make:
    (
      ~dispatch: (. Graph.action) => unit,
      ~onSubmit: unit => unit,
      ~i: Graph.Vertex.t,
      ~j: Graph.Vertex.t,
      ~w: option(float)
    ) =>
    React.element;
};
