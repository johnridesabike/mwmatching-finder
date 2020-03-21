module Table: {
  [@react.component]
  let make:
    (
      ~vertices: array(string),
      ~editVertex: string => unit,
      ~editEdge: (string, string, option(float)) => unit,
      ~mates: Blossom.Match.t(string, Blossom.Match.String.Cmp.identity),
      ~graph: Graph.t,
      ~disabled: bool=?,
      ~style: ReactDOMRe.Style.t=?
    ) =>
    React.element;
};

[@react.component]
let make:
  (
    ~vertices: array(string),
    ~graph: Graph.t,
    ~dispatch: Graph.action => unit,
    ~cardinality: Blossom.Match.cardinality,
    ~setCardinality: (
                       Blossom.Match.cardinality => Blossom.Match.cardinality
                     ) =>
                     unit,
    ~mates: Blossom.Match.t(string, Blossom.Match.String.Cmp.identity),
    ~style: ReactDOMRe.Style.t=?
  ) =>
  React.element;
