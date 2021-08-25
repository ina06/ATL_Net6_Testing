namespace Net6ClassLib
{
    public delegate void CalcResultDelegate();

    public class CalcResult
    {
        public event CalcResultDelegate CalcResultChanged;

        protected virtual void OnCalcResultChanged()
        {
            CalcResultChanged?.Invoke();
        }

    }
}
