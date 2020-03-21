[@react.component]
let make:
  (
    ~graph: Graph.t,
    ~mates: Graph.Mates.t,
    ~width: int,
    ~height: int,
    ~children: React.element=?,
    ~style: ReactDOMRe.Style.t=?
  ) =>
  React.element;
