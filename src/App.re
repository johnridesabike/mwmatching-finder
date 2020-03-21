module Toggleable = {
  type toggled =
    | Open
    | Closed;

  [@react.component]
  let make = (~title, ~children, ~default=Open) => {
    let (toggle, setToggle) = React.useState(() => default);
    <div className="how-to">
      <h2>
        {switch (toggle) {
         | Open =>
           <button
             className="button-ghost" onClick={_ => setToggle(_ => Closed)}>
             title->React.string
             " "->React.string
             <Icons.ChevronDown className="icon" />
           </button>
         | Closed =>
           <button
             className="button-ghost" onClick={_ => setToggle(_ => Open)}>
             title->React.string
             " "->React.string
             <Icons.ChevronLeft className="icon" />
           </button>
         }}
      </h2>
      {switch (toggle) {
       | Open => children
       | Closed => React.null
       }}
    </div>;
  };
};

module HowTo = {
  [@react.component]
  let make = () =>
    <Toggleable title="How to use the table">
      <p>
        {|Each cell represents an edge (a link) between two vertices
          (e.g., people). A number indicates the weight of the edge. A
          blank cell indicates no edge exists.|}
        ->React.string
      </p>
      <p>
        "An edge highlighted in "->React.string
        <span className="color-green"> "green"->React.string </span>
        {| has been chosen for matching. The vertices connected by that
           edge are mated.|}
        ->React.string
      </p>
      <p>
        {|The blossom algorithm will find the path (a set of mates)
          that has the highest combined weight while including as
          highest number of vertices.|}
        ->React.string
      </p>
      <p>
        "Enabling "->React.string
        <em> "maximum cardinality"->React.string </em>
        {| will tell the algorithm to only accept paths with everyone
          possible, even paths with dramatically lower weights.|}
        ->React.string
      </p>
      <p>
        {|Edit an edge by clicking on its table cell. Edit a vertex by clicking
         on its name.|}
        ->React.string
      </p>
    </Toggleable>;
};

module Credits = {
  [@react.component]
  let make = () =>
    <Toggleable title="Credits" default=Toggleable.Closed>
      <div style=Css.(style([textAlign(`center)]))>
        <h3> "Creator & maintainer"->React.string </h3>
        <p>
          <a href="https://johnridesa.bike/"> "John Jackson"->React.string </a>
        </p>
        <h3> "Software"->React.string </h3>
        <ul
          style=Css.(
            style([
              columnCount(count(2)),
              maxWidth(px(320)),
              margin(`auto),
              padding(px(0)),
              listStyleType(`none),
            ])
          )>
          <li>
            <a href="https://nodejs.org/en/">
              <Icons.NodeJs style=Css.(style([height(em(1.))])) />
              "Node.js"->React.string
            </a>
          </li>
          <li>
            <a href="https://reactjs.org/">
              <Icons.React style=Css.(style([height(em(1.))])) />
              {j|React|j}->React.string
            </a>
          </li>
          <li>
            <a href="https://reasonml.github.io/">
              <Icons.Reason style=Css.(style([height(em(1.))])) />
              {j|Reason|j}->React.string
            </a>
          </li>
          <li>
            <a href="https://github.com/johnridesabike/re-blossom">
              {j|🌺 re-blossom|j}->React.string
            </a>
          </li>
          <li>
            <a href="https://d3js.org/">
              <Icons.D3 style=Css.(style([height(em(1.))])) />
              "D3.js"->React.string
            </a>
          </li>
          <li>
            <a href="https://bucklescript.github.io/">
              {j|BuckleScript|j}->React.string
            </a>
          </li>
          <li>
            <a href="https://github.com/MinimaHQ/re-formality">
              "Formality"->React.string
            </a>
          </li>
          <li>
            <a href="https://localforage.github.io/localForage/">
              "LocalForage"->React.string
            </a>
          </li>
          <li>
            <a href="https://github.com/reasonml-labs/bs-css">
              "bs-css"->React.string
            </a>
          </li>
          <li>
            <a href="https://github.com/aantron/promise">
              "Reason Promise"->React.string
            </a>
          </li>
          <li>
            <a href="https://github.com/glennsl/bs-json">
              "bs-json"->React.string
            </a>
          </li>
          <li>
            <a href="https://parceljs.org/"> "Parcel"->React.string </a>
          </li>
          <li>
            <a href="https://github.com/">
              "git + "->React.string
              <Icons.GitHub style=Css.(style([height(em(1.))])) />
              "GitHub"->React.string
            </a>
          </li>
          <li>
            <a href="https://reacttraining.com/reach-ui/">
              "Reach UI"->React.string
            </a>
          </li>
          <li>
            <a href="https://simpleicons.org/">
              <Icons.SimpleIcons style=Css.(style([height(em(1.))])) />
              "Simple Icons"->React.string
            </a>
          </li>
          <li>
            <a href="https://feathericons.com/">
              "Feather icons"->React.string
            </a>
          </li>
        </ul>
      </div>
    </Toggleable>;
};

module Storage = {
  open LocalForage;
  let graph =
    Record.make(
      Config.make(
        ~name="blossom-playground",
        ~storeName="blossom-playground",
        (),
      ),
      (module Graph),
    );
};

module Main = {
  [@react.component]
  let make = (~graph, ~setShowIntro) => {
    let (graph, dispatch) = React.useReducer(Graph.reducer, graph);
    React.useEffect1(
      () => {
        LocalForage.Record.set(Storage.graph, ~items=graph)->ignore;
        None;
      },
      [|graph|],
    );
    let vertices = Graph.verticesToArray(graph);
    let (cardinality, setCardinality) = React.useState(() => `NotMax);
    let mates = Graph.Mates.useMates(graph, ~cardinality);
    <main role="main" className="main">
      <h1> {j|Maximum Weighted Matching Finder|j}->React.string </h1>
      <p style=Css.(style([textAlign(`center)]))>
        <button className="show-intro" onClick={_ => setShowIntro(_ => true)}>
          "Show introduction"->React.string
        </button>
      </p>
      <div className="layout">
        <GraphTable vertices graph dispatch cardinality setCardinality mates />
        <ForceGraph graph mates width=400 height=400 />
      </div>
      <HowTo />
      <Credits />
      <footer className="footer font-small">
        <p>
          "Copyright \xA9 2020 "->React.string
          <a href="https://johnridesa.bike/"> "John Jackson"->React.string </a>
        </p>
        <p>
          <a href="https://github.com/johnridesabike/mwmatching-finder">
            "Browse the source"->React.string
          </a>
        </p>
        <p>
          "Powered by: "->React.string
          <a href="https://reactjs.org/">
            <Icons.React style=Css.(style([height(em(1.))])) />
            {j|React|j}->React.string
          </a>
          " "->React.string
          <a href="https://reasonml.github.io/">
            <Icons.Reason style=Css.(style([height(em(1.))])) />
            {j|Reason|j}->React.string
          </a>
          "\xa0 "->React.string
          <a href="https://github.com/johnridesabike/re-blossom">
            {j|🌺 re-blossom|j}->React.string
          </a>
        </p>
      </footer>
    </main>;
  };
};

type loaded('a) =
  | Loading
  | Loaded('a);

[@react.component]
let make = () => {
  let (loaded, setLoaded) = React.useState(() => Loading);
  let (showIntro, setShowIntro) = React.useState(() => false);
  React.useEffect0(() => {
    LocalForage.Record.get(Storage.graph)
    ->Promise.Js.fromBsPromise
    ->Promise.Js.toResult
    ->Promise.tapOk(graph => setLoaded(_ => Loaded(graph)))
    ->Promise.tapError(_ => {
        setLoaded(_ => Loaded(SampleData.a));
        setShowIntro(_ => true);
      })
    ->ignore;
    None;
  });
  switch (loaded) {
  | Loading => React.null
  | Loaded(graph) =>
    <React.Fragment>
      <Main graph setShowIntro />
      {switch (showIntro) {
       | false => React.null
       | true =>
         <Dialog
           ariaLabel="Introduction to maximum weighted matching "
           onDismiss={() => setShowIntro(_ => false)}>
           <Intro close={() => setShowIntro(_ => false)} />
         </Dialog>
       }}
    </React.Fragment>
  };
};
