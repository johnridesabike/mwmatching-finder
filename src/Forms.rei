module VertexEditor: {
  [@react.component]
  let make:
    (
      ~dispatch: Graph.action => unit,
      ~onSubmit: unit => unit,
      ~name: string,
      ~names: Belt.Set.String.t
    ) =>
    React.element;
};

module VertexAdder: {
  [@react.component]
  let make:
    (
      ~dispatch: Graph.action => unit,
      ~onSubmit: unit => unit,
      ~names: Belt.Set.String.t
    ) =>
    React.element;
};

module EdgeSetter: {
  [@react.component]
  let make:
    (
      ~dispatch: Graph.action => unit,
      ~onSubmit: unit => unit,
      ~i: string,
      ~j: string,
      ~w: option(float)
    ) =>
    React.element;
};
